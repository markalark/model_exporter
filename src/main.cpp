#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h> // memory management
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "StringUtils.h"
#include "AssimpInterpretor.h"
#include "Exporter.h"

//
int main(int argc, char** argv) {
    // if no args are given (only the executable file is passed in), quit and print usage
    if (argc == 1) {
        std::cout << "Hey, you need to give the input file, man." << std::endl;
        std::cout << "Usage: model_exporter.exe \"path/to/file.whatever\" \"optional/output/directory\"" << std::endl;
        return 0;
    }
    // parse the arguments (required: input filepath, optional: output directory)
    std::string input_file = argc > 1 ? argv[1] : "";

    // get the name from the file (remove the path and extension)
    std::vector<std::string> filepath_pieces = split(input_file, {'/', '\\'});
    std::string filename = filepath_pieces.back();
    std::string input_directory = input_file.substr(0, input_file.size() - filename.size());
    std::vector<std::string> file_pieces = split(filename, '.');
    std::string model_name = !file_pieces.empty() ? file_pieces[0] : std::string("couldnt_get_name_from_file");

    // default output directory is the input directory
    std::string output_directory = argc > 2 ? argv[2] : input_directory;
    if (output_directory.back() != '/' && output_directory.back() != '\\') {
        output_directory += '/';
    }

    // Create an instance of the Importer class
    Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 3);

    // make scene
    const aiScene* scene = importer.ReadFile(input_file,
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    // If the import failed, report it
    if (!scene) {
        std::cout << importer.GetErrorString();
        return false;
    }

    // extract the data from the scene
    mdx::AssimpInterpretor interpretor(scene);
    auto model = interpretor.get_model();
    auto skeleton = interpretor.get_skeleton();
    auto animation = interpretor.get_animation();

    // write the model and skeleton
    std::cout << "creating model file: " << output_directory + model_name + ".moe" << std::endl;
    std::ofstream model_file;
    model_file.open(output_directory + model_name + ".moe", std::ofstream::out);
    mdx::Exporter::output_model(*model, *skeleton, model_file);
    mdx::Exporter::output_skeleton(*skeleton, model_file);

    // write the animation file
    if (animation->has_keys()) {
        std::cout << "creating animation file: " << output_directory + model_name + ".mad" << std::endl;
        std::ofstream animation_file;
        animation_file.open(output_directory + model_name + ".mad", std::ofstream::out);
        mdx::Exporter::output_animation(*animation, *skeleton, animation_file);
    }
    return 0;
}
