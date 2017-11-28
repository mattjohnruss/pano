#include <level.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <random>

Level::Level(const std::string &level_path)
{
    for(unsigned i = 0; i < 10; ++i)
    {
        std::string filename = "data/textures/plasma/plasma_" + std::to_string(i) + ".jpg";
        //std::string filename = "data/textures/numbers/" + std::to_string(i) + ".jpg";
        textures_.emplace_back(std::make_unique<Texture2D>(filename));
    }

    std::ifstream level_file(level_path);
    std::string line;

    std::vector<GLfloat> vertex_data;
    std::vector<GLuint> indices;
    std::vector<GLfloat> tex_ids;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<unsigned> dist(0,9);

    // there are 4 vertices per tile
    constexpr unsigned vertices_per_tile = 4;

    // hardcoded texture coords (for now)
    constexpr float tex_coords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };

    std::stringstream line_stream;

    unsigned n_tile = 0;
    for(; std::getline(level_file, line, '\n'); ++n_tile)
    {
        float x, y, z;
        float width, height;
        float r, g, b, a;

        line_stream.str("");
        line_stream.clear();

        line_stream << line;
        line_stream >> x >> y >> z
                    >> width >> height
                    >> r >> g >> b >> a;

        vertex_data.push_back(x - 0.5*width);
        vertex_data.push_back(y - 0.5*height);
        vertex_data.push_back(z);
        vertex_data.push_back(r);
        vertex_data.push_back(g);
        vertex_data.push_back(b);
        vertex_data.push_back(a);

        vertex_data.push_back(x + 0.5*width);
        vertex_data.push_back(y - 0.5*height);
        vertex_data.push_back(z);
        vertex_data.push_back(r);
        vertex_data.push_back(g);
        vertex_data.push_back(b);
        vertex_data.push_back(a);

        vertex_data.push_back(x + 0.5*width);
        vertex_data.push_back(y + 0.5*height);
        vertex_data.push_back(z);
        vertex_data.push_back(r);
        vertex_data.push_back(g);
        vertex_data.push_back(b);
        vertex_data.push_back(a);

        vertex_data.push_back(x - 0.5*width);
        vertex_data.push_back(y + 0.5*height);
        vertex_data.push_back(z);
        vertex_data.push_back(r);
        vertex_data.push_back(g);
        vertex_data.push_back(b);
        vertex_data.push_back(a);

        indices.push_back(n_tile*vertices_per_tile + 0);
        indices.push_back(n_tile*vertices_per_tile + 1);
        indices.push_back(n_tile*vertices_per_tile + 2);
        indices.push_back(n_tile*vertices_per_tile + 2);
        indices.push_back(n_tile*vertices_per_tile + 3);
        indices.push_back(n_tile*vertices_per_tile + 0);

        const GLfloat tex_id = (GLfloat)dist(rng);
        tex_ids.push_back(tex_id);
        tex_ids.push_back(tex_id);
        tex_ids.push_back(tex_id);
        tex_ids.push_back(tex_id);
    }

    level_file.close();

    vbo_.set_data(&vertex_data[0], vertex_data.size());
    uv_vbo_.set_data(&tex_coords[0], 4*2, n_tile);
    tex_id_vbo_.set_data(&tex_ids[0], tex_ids.size());
    ibo_.set_data(indices);

    //vao_.add_buffer(vbo_, { 0, 2, 1 }, { 3, 4, 2 }, { 0, 3, 7 }, 9);
    vao_.add_buffer(vbo_, { 0, 2 }, { 3, 2 }, { 0, 3 }, 7);
    vao_.add_buffer(uv_vbo_, 1, 2);
    vao_.add_buffer(tex_id_vbo_, 3, 1);
    vao_.add_index_buffer(ibo_);

    //vao_.bind();
}

void Level::draw(Shader &shader)
{
    shader.use();

    //textures_[0]->use(GL_TEXTURE0);
    //shader.set_int("u_texture", 0);

    for(unsigned i = 0; i < 10; ++i)
    {
        textures_[i]->use(GL_TEXTURE0 + i);
        shader.set_int(("u_texture[" + std::to_string(i) + "]").c_str(), i);
    }

    vao_.bind();
    glDrawElements(GL_TRIANGLES, ibo_.size(), GL_UNSIGNED_INT, 0);
    vao_.unbind();
}
