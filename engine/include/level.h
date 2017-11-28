#pragma once

#include <buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>
#include <shader.h>
#include <texture.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <memory>

class Level
{
public:
    struct TileVertex
    {
        glm::vec3 position;
        glm::vec3 colour;
    };

    Level(const std::string &level_path);

    void draw(Shader &shader);

private:
    //std::vector<TileVertex> tile_vertices_;
    //std::vector<float> tile_vertices_;

    std::vector<std::unique_ptr<Texture2D>> textures_;

    Buffer vbo_;
    Buffer uv_vbo_;
    Buffer tex_id_vbo_;
    IndexBuffer ibo_;
    VertexArray vao_;
};
