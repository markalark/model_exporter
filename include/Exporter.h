/*
 * File:   Exporter.h
 * Author: Markalark
 *
 */

#ifndef EXPORTER_H
#define	EXPORTER_H

#include <fstream>
#include "Model.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Matrix4x4.h"

namespace mdx {
    class Exporter {
    public:
        static void output_model(const Model &model, const Skeleton &skeleton, std::ofstream &output);
        static void output_skeleton(const Skeleton &skeleton, std::ofstream &output);
        static void output_animation(const Animation &animation, const Skeleton &skeleton, std::ofstream &output);

    private:
        static void output_mesh(const Mesh &mesh, std::ofstream &output);
        static void output_matrix(const Matrix4x4 &matrix, std::ofstream &output);
    };
};

#endif	/* EXPORTER_H */

