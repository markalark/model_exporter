#ifndef MESH_H
#define	MESH_H

#include <string>
#include <vector>
#include "Matrix4x4.h"
#include "Face.h"

//
namespace mdx {
    class Mesh {
    public:
        Mesh(std::string name, std::vector<Face> faces, std::string texture_name, Matrix4x4 transform) :
        name(name), faces(faces), texture_name(texture_name), transform(transform) { }

        //
        std::string get_name() const {
            return name;
        }
        const std::vector<Face> &get_faces() const {
            return faces;
        }
        std::string get_texture_name() const {
            return texture_name;
        }
        const Matrix4x4 &get_transform() const {
            return transform;
        }
    private:
        std::string name;
        std::vector<Face> faces;
        std::string texture_name;
        Matrix4x4 transform;
    };
};
#endif

