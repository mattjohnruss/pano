#include <level.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>
#include <cmath>

const double pi = std::acos(-1);

Level::Level(const std::string &level_path)
    //: rd_(), gen_(rd_()), dist_(0.0, 2.0*pi)
{
    for(unsigned i = 0; i < 10; ++i)
    {
        //std::string filename = "data/textures/plasma/plasma_" + std::to_string(i) + ".jpg";
        std::string filename = "data/textures/numbers/" + std::to_string(i) + ".jpg";
        textures_.emplace_back(std::make_unique<Texture2D>(filename));
    }

    std::ifstream level_file(level_path);
    std::string line;

    //std::random_device rd;
    //std::mt19937 rng(rd());
    //std::uniform_real_distribution<float> dist(0,2.0*pi);

    std::stringstream line_stream;

    unsigned n_tile = 0;
    for(; std::getline(level_file, line, '\n'); ++n_tile)
    {
        float x, y, z;
        float width, height;
        float r, g, b, a;

        glm::vec3 position;
        glm::vec2 size;
        glm::vec4 colour;

        line_stream.str("");
        line_stream.clear();

        line_stream << line;
        line_stream >> position.x >> position.y >> position.z
                    >> size.x >> size.y
                    >> colour.r >> colour.g >> colour.b >> colour.a;

        tile_vertices_.emplace_back(position, size, colour, textures_[0].get()); 
    }

    level_file.close();

    //phases_.resize(n_tile);
    //frequencies_.resize(n_tile);
    //original_zs_.resize(n_tile);

    //for(unsigned i = 0; i < n_tile; ++i)
    //{
        //phases_[i] = dist(rng);
        //frequencies_[i] = dist(rng);

        //original_zs_[i] = tile_vertices_[i].position.z;
    //}
}

void Level::draw(Shader &shader)
{
    shader.use();

    const unsigned n_tile = tile_vertices_.size();
    //const double t = glfwGetTime();
    
    //for(unsigned i = 0; i < n_tile; ++i)
    //{
        //tile_vertices_[i].position.z = original_zs_[i] + 0.1*std::sqrt(frequencies_[i]*t + phases_[i]);
    //}

    renderer_.begin();
    renderer_.submit(tile_vertices_);
    renderer_.end();
    renderer_.flush();

    //textures_[0]->use(GL_TEXTURE0);
    //shader.set_int("u_texture", 0);

    //static bool setup_done = false;

    //for(unsigned i = 0; i < 10; ++i)
    //{
        //textures_[i]->use(GL_TEXTURE0 + i);
        //shader.set_int(("u_texture[" + std::to_string(i) + "]").c_str(), i);

        //setup_done = true;
    //}
}
