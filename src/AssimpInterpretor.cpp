#include <iostream>
#include <assert.h>
#include <string>
#include <fstream>
#include "StringUtils.h"
#include "AssimpInterpretor.h"
#include "Matrix4x4.h"
#include "Vec3.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../include/assimp/version.h"
#include "glm_headers.h"
#include "Mesh.h"
#define PI 3.14159265359f

//
namespace mdx {
    const Matrix4x4 AssimpInterpretor::INVERSE_ASSIMP_TRANSFORM = Matrix4x4(glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(1, 0, 0)));
    AssimpInterpretor::AssimpInterpretor(const aiScene *scene) :
        scene(scene),
        model(new Model()),
        skeleton(new Skeleton()),
        animation(new Animation()) {
        interpret_node(scene->mRootNode, INVERSE_ASSIMP_TRANSFORM);
        interpret_animations();
    }

    //
    std::vector<Face> AssimpInterpretor::interpret_faces(const aiMesh* assimp_mesh, const Matrix4x4 &transform) {
        // for each assimp face...
        std::vector<Face> faces(assimp_mesh->mNumFaces);
        for (unsigned int i = 0; i < assimp_mesh->mNumFaces; ++i) {
            const aiFace& assimp_face = assimp_mesh->mFaces[i];
            // grab verts of face
            std::vector<int> face_vertices;
            for (unsigned int j = 0; j < assimp_face.mNumIndices; j++) {
                const unsigned int assimp_index = assimp_face.mIndices[j];

                // add new vertex
                Vertex vertex;
                vertex.set_position(transform * Vec3f(assimp_mesh->mVertices[assimp_index]));
                vertex.set_normal(Vec3f(assimp_mesh->mNormals[assimp_index]));
                if (assimp_mesh->mTextureCoords[0]) {
                    vertex.set_uv(Vec2f(assimp_mesh->mTextureCoords[0][assimp_index]));
                }

                // add vertex to the model
                int index = model->add_vertex(vertex);

                // allow the model's vertex index to be accessible by assimp's mesh and vertex id
                register_vertex_id(assimp_mesh, assimp_index, index);

                // add the vertex reference to the face
                face_vertices.push_back(index);
            }

            // add face
            faces[i] = Face(face_vertices);
        }
        return faces;
    }

    //
    std::shared_ptr<Mesh> AssimpInterpretor::interpret_mesh(std::string name_prefix, const aiMesh *assimp_mesh, const Matrix4x4 &transform) {
        // establish the group name
        std::string mesh_name = assimp_mesh->mName.C_Str();
        std::string group_name = !mesh_name.empty() ? name_prefix + "_" + mesh_name : name_prefix;

        // get texture file
        aiString texture_filename;
        auto material = this->scene->mMaterials[assimp_mesh->mMaterialIndex];
        aiReturn found_texture = material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_filename);
        if (found_texture == AI_SUCCESS) {
            std::cout << "texture file: " << std::string(texture_filename.C_Str()) << std::endl;
        }
        // remove file extention
        std::vector<std::string> file_pieces = split(texture_filename.C_Str(), '.');
        std::string texture_name = (!file_pieces.empty()) ? file_pieces.front() : "";

        // get the faces of the mesh (and store the mesh vertices)
        const std::vector<Face> &faces = interpret_faces(assimp_mesh, transform);

        // interpret the bones of this mesh, storing their details into the skeleton
        interpret_mesh_bones(assimp_mesh);

        // create and add the mesh
        return std::shared_ptr<Mesh>(new Mesh(group_name, faces, texture_name, transform));
    }

    //
    void AssimpInterpretor::interpret_mesh_bones(const aiMesh *assimp_mesh) {
        for (int i = 0; i < assimp_mesh->mNumBones; i++) {
            aiBone *assimp_bone = assimp_mesh->mBones[i];
            std::string bone_name = assimp_bone->mName.C_Str();
            std::shared_ptr<Skeleton::Bone> bone = skeleton->add_bone(bone_name);

            // the inverse bind pose
            bone->set_inverse_bind_pose(Matrix4x4(assimp_bone->mOffsetMatrix));

            // add the weights
            for (int j = 0; j < assimp_bone->mNumWeights; j++) {
                aiVertexWeight assimp_weight = assimp_bone->mWeights[j];
                int vertex_id = get_vertex_id(assimp_mesh, assimp_weight.mVertexId);
                model->add_weight(vertex_id, bone, assimp_weight.mWeight);
            }
        }
    }

    //
    void AssimpInterpretor::interpret_node(const aiNode *assimp_node, const Matrix4x4 &parent_transform) {
        std::string node_name = assimp_node->mName.C_Str();
        Matrix4x4 local_transform(assimp_node->mTransformation);
        Matrix4x4 global_transform(parent_transform * local_transform);

        // add node's meshes
        for (int i = 0; i < assimp_node->mNumMeshes; i++) {
            auto *assimp_mesh = scene->mMeshes[assimp_node->mMeshes[i]];
            auto mesh = interpret_mesh(node_name, assimp_mesh, global_transform);
            model->add_mesh(mesh);
        }

        // children of the armature are bones
        bool children_are_bones = is_armature(assimp_node);

        // add children
        if (children_are_bones) {
            for (unsigned int i = 0; i < assimp_node->mNumChildren; i++) {
                interpret_bone(assimp_node->mChildren[i], global_transform, std::shared_ptr<Skeleton::Bone>(nullptr));
            }
        } else {
            for (unsigned int i = 0; i < assimp_node->mNumChildren; i++) {
                interpret_node(assimp_node->mChildren[i], global_transform);
            }
        }
    }

    //
    void AssimpInterpretor::interpret_bone(const aiNode *assimp_bone, const Matrix4x4 &parent_transform, std::shared_ptr<Skeleton::Bone> parent_bone) {
        std::string bone_name = assimp_bone->mName.C_Str();
        Matrix4x4 assimp_local_transform(assimp_bone->mTransformation);
        Matrix4x4 global_transform(parent_transform * assimp_local_transform);
        Matrix4x4 inverse_parent_global = parent_bone != nullptr ? parent_bone->get_global_transform().get_inverse() : Matrix4x4(1.0f);
        Matrix4x4 local_transform = inverse_parent_global * global_transform;

        // create and add the bone
        std::shared_ptr<Skeleton::Bone> bone = skeleton->add_bone(bone_name);
        bone->set_local_transform(local_transform);
        bone->set_global_transform(global_transform);
        skeleton->attach_bone(bone, parent_bone);

        // add bone's meshes
        for (int i = 0; i < assimp_bone->mNumMeshes; i++) {
            auto *assimp_mesh = scene->mMeshes[assimp_bone->mMeshes[i]];
            auto mesh = interpret_mesh(bone_name, assimp_mesh, global_transform);
            model->add_mesh(mesh);
        }

        // interpret the children bones
        for (unsigned int i = 0; i < assimp_bone->mNumChildren; i++) {
            interpret_bone(assimp_bone->mChildren[i], global_transform, bone);
        }
    }

    //
    int AssimpInterpretor::get_vertex_id(const aiMesh *assimp_mesh, int assimp_vertex_id) {
        return model_vertex_ids[assimp_mesh][assimp_vertex_id];
    }
    void AssimpInterpretor::register_vertex_id(const aiMesh *assimp_mesh, int assimp_vertex_id, int model_vertex_id) {
        model_vertex_ids[assimp_mesh][assimp_vertex_id] = model_vertex_id;
    }

    //
    bool AssimpInterpretor::is_armature(const aiNode *node) {
        std::string node_name = node->mName.C_Str();
        return node_name == std::string("Armature");
    }

    //
    void AssimpInterpretor::interpret_animations() {
        // for each animation...
        for (int i = 0; i < scene->mNumAnimations; i++) {
            aiAnimation *scene_anim = scene->mAnimations[i];
            // each node...
            for (int j = 0; j < scene_anim->mNumChannels; j++) {
                aiNodeAnim *anim_node = scene_anim->mChannels[j];
                std::string node_name = anim_node->mNodeName.C_Str();
                std::shared_ptr<Skeleton::Bone> bone = skeleton->get_bone(node_name);
                // if node is a bone...
                if (bone != nullptr) {
                    // get position changes
                    for (int k = 0; k < anim_node->mNumPositionKeys; k++) {
                        aiVectorKey position_key = anim_node->mPositionKeys[k];
                        double time = position_key.mTime;
                        Vec3f offset(position_key.mValue);
                        // add key to animation
                        animation->add_offset(time, bone, offset);
                    }

                    // get rotation changes
                    for (int k = 0; k < anim_node->mNumRotationKeys; k++) {
                        aiQuatKey rotation_key = anim_node->mRotationKeys[k];
                        float time = rotation_key.mTime;
                        aiQuaternion assimp_orientation = rotation_key.mValue;
                        glm::quat orientation = glm::quat(assimp_orientation.w, glm::vec3(assimp_orientation.x, assimp_orientation.y, assimp_orientation.z));
                        // add key to animation
                        animation->add_orientation(time, bone, orientation);
                    }

                    // get scale changes
                    for (int k = 0; k < anim_node->mNumScalingKeys; k++) {
                        aiVectorKey scaling_key = anim_node->mScalingKeys[k];
                        float time = scaling_key.mTime;
                        Vec3f scale(scaling_key.mValue);
                        // add key to animation
                        animation->add_scale(time, bone, scale);
                    }
                }
            }
        }
    }

    //
    Matrix4x4 AssimpInterpretor::get_pose_transform(const std::vector<Vertex::BoneWeight> &bone_weights) {
        Matrix4x4 mesh_to_pose(0.0f);
        for (auto &bone_weight : bone_weights) {
            mesh_to_pose += bone_weight.bone->get_global_transform() * bone_weight.bone->get_inverse_bind_pose() * bone_weight.weight;
        }
        return mesh_to_pose;
    }
};