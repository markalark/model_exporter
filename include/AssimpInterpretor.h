#ifndef PARSER_H
#define	PARSER_H

#include <memory>
#include <map>
#include "Model.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Mesh.h"
#include "Face.h"
#include "Vertex.h"

// parses a model file into the format used for my engine
namespace mdx {
    class AssimpInterpretor {
    public:
        AssimpInterpretor(const aiScene *scene);
        std::shared_ptr<Model> get_model() {
            return model;
        }
        std::shared_ptr<Skeleton> get_skeleton() {
            return skeleton;
        }
        std::shared_ptr<Animation> get_animation() {
            return animation;
        }
    private:
        std::vector<Face> interpret_faces(const aiMesh* assimp_mesh, const Matrix4x4 & transform);
        std::shared_ptr<Mesh> interpret_mesh(std::string name, const aiMesh* assimp_mesh, const Matrix4x4 & transform);
        void interpret_mesh_bones(const aiMesh * assimp_mesh);
        void interpret_node(const aiNode *assimp_node, const Matrix4x4 & parent_matrix);
        void interpret_bone(const aiNode *assimp_bone, const Matrix4x4 &parent_transform, std::shared_ptr<Skeleton::Bone> parent_bone);
        void interpret_animations();
        int get_vertex_id(const aiMesh *assimp_mesh, int assimp_vertex_id);
        void register_vertex_id(const aiMesh *assimp_mesh, int assimp_vertex_id, int model_vertex_id);
        bool is_armature(const aiNode * node);
        Matrix4x4 get_pose_transform(const std::vector<Vertex::BoneWeight> &bone_weights);
        const aiScene *scene;
        std::shared_ptr<Model> model;
        std::shared_ptr<Skeleton> skeleton;
        std::shared_ptr<Animation> animation;
        std::map<const aiMesh *, std::map<int, int> > model_vertex_ids;
        const int MAX_WEIGHTS = 3;
        /** Using this on a transform given from assimp will revert the axis alignment
         * to reflect exactly what you see in the modeling program */
        static const Matrix4x4 INVERSE_ASSIMP_TRANSFORM;
    };
};

#endif