#pragma once

#include <mesh_vertex.h>
#include <buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>

#include <glad/glad.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

// forward declaration of Model
class Model;
class Shader;

// simple texture data structure for use in Mesh
struct MeshTexture
{
    // maybe use GLuint instead of unsigned?
    unsigned id;
    aiTextureType type;
};

// Mesh class
//
// The Model object which creates a Mesh, which is passed to the constructor,
// must be kept alive for the lifetime of the Mesh, since the Model owns the
// storage for the textures of all it's Meshes.
class Mesh
{
public:
    // constructor which processes assimp data structures into ours
    Mesh(const aiMesh *const mesh,
         const aiScene *const scene,
         const std::string &directory,
         Model &parent_model);

    // delete the copy constructor and assignment operator to make sure we
    // don't copy anything around by accident
    Mesh(const Mesh &) = delete;
    Mesh& operator=(const Mesh &) = delete;

    void draw() const {}

    // draw the mesh with a specified gl draw mode, defaulting to triangles
    void draw(const Shader &shader, const GLenum mode = GL_TRIANGLES) const;

private:
    // load textures specified by the assimp material structure
    void load_material_textures(
            aiMaterial *material,
            aiTextureType tex_type,
            Model &parent_model);

    // buffer and array objects
    Buffer vbo_;
    IndexBuffer ebo_;
    VertexArray vao_;

    // storage for the vertices, indices and textures of the mesh
    std::vector<MeshVertex> vertices_;
    std::vector<unsigned> indices_;
    std::vector<const MeshTexture*> textures_;

    // directory from which the assets are loaded
    // gets around relative/absolute path problems
    std::string directory_;
};
