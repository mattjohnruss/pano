#pragma once

#include <buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>
#include <shader.h>
#include <texture.h>
#include <batch_renderer.h>

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <memory>
#include <random>

class Level
{
public:
    //struct TileVertex
    //{
        //glm::vec3 position;
        //glm::vec3 colour;
    //};

    Level(const std::string &level_path);

    void draw(Shader &shader);

private:
    std::vector<Renderable2D> tile_vertices_;
    std::vector<std::unique_ptr<Texture2D>> textures_;
    BatchRenderer renderer_;

    //std::random_device rd_;
    //std::mt19937 gen_;
    //std::uniform_real_distribution<float> dist_;

    std::vector<float> phases_;
    std::vector<float> frequencies_;
    std::vector<float> original_zs_;
};
