/*
 * File:   Face.h
 * Author: Markalark
 *
 */

#ifndef FACE_H
#define	FACE_H
#include <vector>

//
namespace mdx {
    class Face {
    public:
        Face() { }
        Face(std::vector<int> vertex_indices) : vertex_indices(vertex_indices) { }
        const std::vector<int> &get_vertex_indices() const {
            return vertex_indices;
        }
    private:
        std::vector<int> vertex_indices;
    };
};

#endif	/* FACE_H */

