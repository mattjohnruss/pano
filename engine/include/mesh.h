#pragma once

#include <shader.h>

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

// forward declaration of Model
class Model;

// simple texture data structure for use in Mesh
struct MeshTexture
{
    // maybe use GLuint instead of unsigned?
    unsigned id;
    aiTextureType type;
};

// vertex data structure
// stores the position vector, normal vector and texture coords of a vertex
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

// Mesh class
class Mesh
{
public:
    // constructor which processes assimp data structures into ours
    Mesh(const aiMesh * const mesh,
         const aiScene * const scene,
         const std::string &directory,
         Model &parent_model);

    // delete the copy constructor and assignment operator
    Mesh(const Mesh &) = delete;
    Mesh& operator=(const Mesh &) = delete;

    // draw the mesh
    void draw(const Shader &shader) const;

private:
    // setup the gl buffers for the mesh
    void setup_buffers();

    // load textures specified by the assimp material structure
    void load_material_textures(
            aiMaterial *material,
            aiTextureType tex_type,
            Model &parent_model);

    // storage for the vertices, indices and textures of the mesh
    std::vector<Vertex> vertices_;
    std::vector<unsigned> indices_;
    std::vector<const MeshTexture*> textures_;

    // directory from which the assets are loaded
    // gets around relative/absolute path problems
    std::string directory_;

    // handles for the gl buffer objects
    unsigned VAO_;
    unsigned VBO_;
    unsigned EBO_;
};
