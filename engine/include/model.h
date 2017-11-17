#pragma once

#include <mesh.h>
#include <shader.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Model class
// loads a model (using assimp), storing data in instances of our Mesh class
class Model
{
public:
    // allow the Mesh class access to the Model's global texture hashmap
    //friend void Mesh::load_material_textures(aiMaterial*, aiTextureType, Model&);

    // constructor
    Model(const std::string &path);

    // destructor
    ~Model();

    const std::unordered_map<std::string, MeshTexture> &textures() const
    {
        return textures_;
    }

    std::unordered_map<std::string, MeshTexture> &textures()
    {
        return textures_;
    }

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
