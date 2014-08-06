/*
 * File:   Matrix4x4.h
 * Author: Markalark
 *
 * Created on July 29, 2014, 7:37 PM
 */

#ifndef MATRIX4X4_H
#define	MATRIX4X4_H
#include <iostream>
#include "Vec3.h"
#include "glm_headers.h"
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "../include/assimp/version.h"

//
class Matrix4x4 {
public:
    // default matrix
    Matrix4x4() { }

    // matrix from quaternion
    Matrix4x4(glm::quat quaternion) : data(glm::mat4_cast(quaternion)) { }

    // matrix from scale factor
    Matrix4x4(float scale) : data(scale) { }

    // matrix from glm
    Matrix4x4(glm::mat4 matrix) : data(matrix) { }

    // matrix from translation
    static Matrix4x4 Translation(Vec3f translation) {
        return Matrix4x4(glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z)));
    }

    // matrix from rotation
    static Matrix4x4 Rotation(float radians, Vec3f axis) {
        return Matrix4x4(glm::rotate(glm::mat4(1.0f), radians, glm::vec3(axis.x, axis.y, axis.z)));
    }

    // matrix from scale
    static Matrix4x4 Scale(Vec3f scale) {
        return Matrix4x4(glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z)));
    }

    // matrix from aimatrix
    Matrix4x4(const aiMatrix4x4 &original) :
    data(original.a1, original.b1, original.c1, original.d1,
         original.a2, original.b2, original.c2, original.d2,
         original.a3, original.b3, original.c3, original.d3,
         original.a4, original.b4, original.c4, original.d4) { }

    //
    Matrix4x4 operator*(const Matrix4x4 &matrix) const {
        Matrix4x4 result = Matrix4x4(data * matrix.data);
        return result;
    }
    Matrix4x4 operator*(const aiMatrix4x4 &matrix) const {
        Matrix4x4 result = data * Matrix4x4(matrix).data;
        return result;
    }
    Matrix4x4 operator*(float value) const {
        return data * value;
    }
    Matrix4x4 operator+(const Matrix4x4 &matrix) const {
        Matrix4x4 result = data + matrix.data;
        return result;
    }
    void operator+=(const Matrix4x4 &matrix) {
        data += matrix.data;
    }
    Vec3f operator*(const Vec3f &vector) const {
        glm::vec4 result = data * glm::vec4(vector.x, vector.y, vector.z, 1);
        return Vec3f(result.x, result.y, result.z);
    }
    glm::mat4::col_type &operator[](int index) {
        return data[index];
    }

    //
    void decompose(Vec3f &scale, glm::quat &rotation, Vec3f &offset) const {
        // get the offset
        offset = get_translation();

        // convert to a rotation matrix
        Matrix4x4 rotation_matrix = this->get_rotation();
        rotation = rotation_matrix.to_quaternion();

        // get the scale before any rotation was applied
        Matrix4x4 scale_matrix(rotation_matrix.get_inverse() * data);
        scale = scale_matrix.get_scale();
    }

    //
    Vec3f get_scale() const {
        Vec3f scaling;
        scaling.x = glm::length(glm::vec3(data[0][0], data[1][0], data[2][0]));
        scaling.y = glm::length(glm::vec3(data[0][1], data[1][1], data[2][1]));
        scaling.z = glm::length(glm::vec3(data[0][2], data[1][2], data[2][2]));
        return scaling;
    }

    //
    Vec3f get_translation() const {
        return Vec3f(data[3]);
    }

    // print to standard out
    void print() const {
        std::cout.precision(3);
        std::cout << std::fixed;
        std::cout << data[0][0] << "\t\t" << data[1][0] << "\t\t" << data[2][0] << "\t\t" << data[3][0] << std::endl;
        std::cout << data[0][1] << "\t\t" << data[1][1] << "\t\t" << data[2][1] << "\t\t" << data[3][1] << std::endl;
        std::cout << data[0][2] << "\t\t" << data[1][2] << "\t\t" << data[2][2] << "\t\t" << data[3][2] << std::endl;
        std::cout << data[0][3] << "\t\t" << data[1][3] << "\t\t" << data[2][3] << "\t\t" << data[3][3] << std::endl;
        std::cout << std::endl;
    }

    // turn to a quat
    glm::quat to_quaternion() const {
        return glm::quat_cast(data);
    }

    // remove scale from matrix
    void remove_translation() {
        data[3] = glm::vec4(0, 0, 0, 1);
    }

    // remove scale from matrix
    void remove_scale() {
        Vec3f scaling = get_scale();
        data[0][0] /= scaling.x;
        data[1][0] /= scaling.x;
        data[2][0] /= scaling.x;
        data[0][1] /= scaling.y;
        data[1][1] /= scaling.y;
        data[2][1] /= scaling.y;
        data[0][2] /= scaling.z;
        data[1][2] /= scaling.z;
        data[2][2] /= scaling.z;
    }

    // remove scale from matrix
    void remove_rotation() {
        Vec3f factor = get_scale();
        Vec3f offset = get_translation();
        data = glm::translate(glm::mat4(1.0f), glm::vec3(offset.x, offset.y, offset.z)) *
                glm::scale(glm::mat4(1.0f), glm::vec3(factor.x, factor.y, factor.z));
    }

    // turn to a quat
    Matrix4x4 get_rotation() const {
        Matrix4x4 rotation_matrix(data);
        rotation_matrix.remove_scale();
        rotation_matrix.remove_translation();
        return rotation_matrix;
    }

    // invert
    Matrix4x4 get_inverse() const {
        return Matrix4x4(glm::inverse(data));
    }

    // scale
    void scale(float x, float y, float z) {
        data[0][0] *= x;
        data[1][1] *= y;
        data[2][2] *= z;
    }

    // normalize
    void normalize() {
        double w = data[3][3];
        data[0][3] /= w;
        data[1][3] /= w;
        data[2][3] /= w;
        data[3][3] /= w;
    }
private:
    glm::mat4 data;
};

#endif	/* MATRIX4X4_H */

