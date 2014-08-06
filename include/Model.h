/*
 * File:   Model.h
 * Author: Markalark
 *
 */

#ifndef MODEL_H
#define	MODEL_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "Vertex.h"
#include "Mesh.h"

//
namespace mdx {
    class Model {
    public:
        int add_vertex(Vertex vertex) {
            if (vertex_ids.find(vertex) == vertex_ids.end()) {
                vertices.push_back(vertex);
                vertex_ids[vertex] = vertices.size();
            }
            return vertex_ids[vertex];
        }
        const std::vector<Vertex> &get_vertices() const {
            return vertices;
        }
        void add_mesh(std::shared_ptr<Mesh> mesh) {
            meshes.push_back(mesh);
        }
        const std::vector<std::shared_ptr<Mesh> > &get_meshes() const {
            return meshes;
        }
        void add_weight(int vertex_id, std::shared_ptr<Skeleton::Bone> bone, float weight) {
            vertices[vertex_id - 1].add_weight(bone, weight);
        }
    private:
        std::vector<Vertex> vertices;
        std::unordered_map<Vertex, int, VertexHasher> vertex_ids;
        std::vector<std::shared_ptr<Mesh> > meshes;
    };
};

#endif	/* MODEL_H */

