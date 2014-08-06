/*
 * File:   Exporter.cpp
 * Author: Markalark
 *
 */

#include <fstream>
#include "Exporter.h"

namespace mdx {
    void Exporter::output_model(const Model &model, const Skeleton &skeleton, std::ofstream &output) {
        // positions
        for (auto &vertex : model.get_vertices()) {
            Vec3f position = vertex.get_position();
            output << "v " << position.x << " " << position.y << " " << position.z << "\n";
        }
        // normals
        for (auto &vertex : model.get_vertices()) {
            Vec3f normal = vertex.get_normal();
            output << "vn " << normal.x << " " << normal.y << " " << normal.z << "\n";
        }
        // uv coordinates
        for (auto &vertex : model.get_vertices()) {
            Vec2f uv = vertex.get_uv();
            output << "vt " << uv.x << " " << uv.y << "\n";
        }
        // write the vertex bone influences
        if (skeleton.has_bones()) {
            // vertices can't have more than three bone weights
            int max_bone_weights = 3;
            for (auto &vertex : model.get_vertices()) {
                output << "vb";
                std::vector<Vertex::BoneWeight> bone_weights(vertex.get_weights());
                // sort the most important weights to the top
                std::sort(bone_weights.begin(), bone_weights.end(), [](Vertex::BoneWeight weight, Vertex::BoneWeight other) {
                    return weight.weight > other.weight;
                });
                // fill in blank bone weights until there's at least three in the array
                while (bone_weights.size() < max_bone_weights) {
                    bone_weights.push_back(Vertex::BoneWeight(nullptr, 0));
                }
                // get the total weight of the three most influential bones (for normalization)
                float total_weight = 0;
                for (int i = 0; i < max_bone_weights; i++) {
                    Vertex::BoneWeight bone_weight = bone_weights[i];
                    total_weight += bone_weight.weight;
                }
                // output the normalized bone weights
                for (int i = 0; i < max_bone_weights; i++) {
                    Vertex::BoneWeight bone_weight = bone_weights[i];
                    int bone_id = (bone_weight.bone != nullptr) ? bone_weight.bone->get_id() : 0;
                    output << " " << bone_id << " " << bone_weight.weight / total_weight;
                }
                output << "\n";
            }
        }
        // faces
        for (auto mesh : model.get_meshes()) {
            output_mesh(*mesh, output);
        }
    }

    //
    void Exporter::output_skeleton(const Skeleton &skeleton, std::ofstream &output) {
        for (auto &bone : skeleton.get_bones()) {
            // bone transform
            output << "b ";
            output_matrix(bone->get_local_transform(), output);
            output << "\n";
        }
        for (auto &bone : skeleton.get_bones()) {
            output << "ibp ";
            output_matrix(bone->get_inverse_bind_pose(), output);
            output << "\n";
        }
        for (auto &bone : skeleton.get_bones()) {
            // bone relationships
            int parent_id = bone->has_parent() ? bone->get_parent()->get_id() : -1;
            output << "bp " << bone->get_id() << " " << parent_id;
            output << "\n";
        }
    }

    //
    void Exporter::output_mesh(const Mesh &mesh, std::ofstream & output) {
        //        output << "mesh " << mesh.get_name() << " " << mesh.get_texture_name() << "\n";
        for (auto &face : mesh.get_faces()) {
            std::vector<int> vertex_indices = face.get_vertex_indices();
            output << "f ";
            output << vertex_indices[0] << "/" << vertex_indices[0] << "/" << vertex_indices[0];
            output << " ";
            output << vertex_indices[1] << "/" << vertex_indices[1] << "/" << vertex_indices[1];
            output << " ";
            output << vertex_indices[2] << "/" << vertex_indices[2] << "/" << vertex_indices[2];
            output << "\n";
        }
    }

    //
    void Exporter::output_animation(const Animation &animation, const Skeleton &skeleton, std::ofstream &output) {
        for (float key_time : animation.get_key_times()) {
            output << "kt " << key_time << "\n";
            for (auto &bone : skeleton.get_bones()) {
                // output the bone's pose at the current key time
                output << "b ";
                Matrix4x4 pose_transform = animation.get_pose_transform(key_time, bone);
                output_matrix(pose_transform, output);
                output << "\n";
            }
        }
        output << "loop 1000\n";
    }
    void Exporter::output_matrix(const Matrix4x4 &matrix, std::ofstream &output) {
        Vec3f scale;
        glm::quat orientation;
        Vec3f offset;
        matrix.decompose(scale, orientation, offset);
        output << orientation.w << " " << orientation.x << " " << orientation.y << " " << orientation.z << " ";
        output << offset.x << " " << offset.y << " " << offset.z;
    }
};
