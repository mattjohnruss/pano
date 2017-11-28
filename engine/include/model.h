#pragma once

#include <mesh.h>
#include <shader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// forward delcaration
class Renderer;

// Model class
// loads a model (using assimp), storing data in instances of our Mesh class
//
// A Model object keeps track of the textures all of its Mesh objects use in
// the textures_ map. This is to avoid loading textures multiple times when
// they are used in multiple Meshes. Each Mesh has a vector of pointers to the
// actual textures it uses

class Model
{
public:
    // constructor
    Model(const std::string &path);

    // destructor
    ~Model();

    // return the textures map (const)
    const std::unordered_map<std::string, MeshTexture> &textures() const;

    // return the textures map
    std::unordered_map<std::string, MeshTexture> &textures();

    // return the vector of meshes (const)
    const std::vector<Mesh *>& meshes() const;

    // draw the model with a specified gl draw mode, defaulting to triangles
    void draw(const Shader &shader, const GLenum mode = GL_TRIANGLES) const;

private:
    // vector of sub-meshes making up the model
    std::vector<Mesh*> meshes_;

    // hashmap of textures indexed by their file path
    std::unordered_map<std::string, MeshTexture> textures_;

    // directory from which the assets are loaded
    // gets around relative/absolute path problems (does it?)
    std::string directory_;

    // traverse the scene hierarchy and process each node
    void traverse_and_process(const aiScene *const scene);

    // process a node from an assimp scene
    void process_node(aiNode *node, const aiScene *const scene);
};
