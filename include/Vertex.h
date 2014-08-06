/*
 * File:   Vertex.h
 * Author: Markalark
 *
 */

#ifndef VERTEX_H
#define	VERTEX_H

#include <vector>
#include <unordered_map>
#include <functional>
#include "Vec3.h"
#include "Vec2.h"
#include "Skeleton.h"

//
namespace mdx {
    struct Vertex {
    public:
        void set_position(Vec3f position) {
            this->position = position;
        }
        Vec3f get_position() const {
            return position;
        }
        void set_normal(Vec3f normal) {
            this->normal = normal;
        }
        Vec3f get_normal() const {
            return normal;
        }
        void set_uv(Vec2f uv) {
            this->uv = uv;
        }
        Vec2f get_uv() const {
            return uv;
        }
        bool operator==(const Vertex &other) const {
            return (this->position == other.position &&
                    this->normal == other.normal &&
                    this->uv == other.uv);
        }
        struct BoneWeight {
            BoneWeight(std::shared_ptr<Skeleton::Bone> bone, float weight) : bone(bone), weight(weight) { }
            std::shared_ptr<Skeleton::Bone> bone;
            float weight;
        };
        void add_weight(std::shared_ptr<Skeleton::Bone> bone, float weight) {
            weights.push_back(BoneWeight(bone, weight));
        }
        std::vector<BoneWeight> get_weights() const {
            return weights;
        }
    private:
        Vec3f position;
        Vec3f normal;
        Vec2f uv;
        std::vector<BoneWeight> weights;
    };


    // make the vertex hashable
    struct VertexHasher {
        std::size_t operator()(const Vertex& vertex) const {
            Vec3f position = vertex.get_position();
            Vec3f normal = vertex.get_normal();
            Vec2f uv = vertex.get_uv();
            return (std::hash<float>()(position.x)) ^
                    (std::hash<float>()(position.y)) ^
                    (std::hash<float>()(position.z)) ^
                    (std::hash<float>()(normal.x)) ^
                    (std::hash<float>()(normal.y)) ^
                    (std::hash<float>()(normal.z)) ^
                    (std::hash<float>()(uv.x)) ^
                    (std::hash<float>()(uv.y));
        }
    };
};

#endif	/* VERTEX_H */

