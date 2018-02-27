#include <batch_renderer.h>
#include <iostream>

// for matX operator<<
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

BatchRenderer::BatchRenderer()
    : Renderer(), vbo_(false), index_count(0)
{
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER,
                 n_max_renderables*sizeof(BRVertex),
                 NULL,
                 GL_DYNAMIC_DRAW);

    vbo_.id() = vbo_id;

    // this works by luck - we do not account for the possible
    // padding/alignment within the BRVertex struct
    //              buf,  indices,        sizes,          offsets,        stride
    //vao_.add_buffer(vbo_, { 0, 1, 2, 3 }, { 3, 2, 4, 1 }, { 0, 3, 5, 9 }, 10);

    //// better but messy
    //vao_.add_buffer(vbo_,                            // buf
                    //{ 0, 1, 2, 3 },                  // indices
                    //{ sizeof(BRVertex::position),
                      //sizeof(BRVertex::uv),
                      //sizeof(BRVertex::colour),
                      //sizeof(BRVertex::texture_id)   // sizes
                    //},
                    //{ offsetof(BRVertex, position),
                      //offsetof(BRVertex, uv),
                      //offsetof(BRVertex, colour),
                      //offsetof(BRVertex, texture_id) // offsets
                    //},
                    //sizeof(BRVertex));               // stride

    // messy but using new reflection stuff
    //vao_.add_buffer(vbo_,                            // buf
                    //{ 0, 1, 2, 3 },                  // indices
                    //{ (int)BRVertex::r.sizes[0],
                      //(int)BRVertex::r.sizes[1],
                      //(int)BRVertex::r.sizes[2],
                      //(int)BRVertex::r.sizes[3]    // sizes
                    //},
                    //{ BRVertex::r.offsets[0],
                      //BRVertex::r.offsets[1],
                      //BRVertex::r.offsets[2],
                      //BRVertex::r.offsets[3]  // offsets
                    //},
                    //BRVertex::r.size());        // stride

    // awesome version using new reflection stuff
    //vao_.add_buffer<BRVertex>(vbo_, { 0, 1, 2, 3 });
    vao_.add_buffer(vbo_, { 0, 1, 2, 3 }, BRVertex::r);

    const unsigned n_max_indices = 6*n_max_renderables;

    const GLuint base_indices[] = { 0, 1, 2, 2, 3, 0 };

    std::vector<GLuint> indices(n_max_indices);

    for(unsigned i = 0; i < n_max_renderables; ++i)
    {
        for(unsigned j = 0; j < 6; ++j)
        {
            indices[6*i + j] = 4*i + base_indices[j];
        }
    }

    ibo_.set_data(indices);
    vao_.add_index_buffer(ibo_);
}

BatchRenderer::~BatchRenderer()
{
}

void BatchRenderer::begin()
{
    vbo_.bind();
    vertex_map = vbo_.map_write_only<BRVertex>();
}

void BatchRenderer::end()
{
    vertex_map = nullptr;
    vbo_.unmap();
    vbo_.unbind();
}

void BatchRenderer::submit(const Renderable2D *const renderable)
{
    // check that begin() has been called before submit()
    assert(vertex_map != nullptr);

    if(index_count >= n_max_renderables)
    {
        //std::cout << "index_count = " << index_count << '\n'
                  //<< "n_max_renderables = " << n_max_renderables << '\n'
                  //<< "flushing early\n";
        end();
        flush();
        begin();
    }

    //const glm::vec3 &position = renderable->position;
    //const glm::vec2 &size     = renderable->size;
    //const glm::vec4 &colour   = renderable->colour;
    //const GLuint &texture_id  = renderable->texture->id();

    //const glm::vec3 position = renderable->position();
    //const glm::vec2 size     = renderable->size();
    //const glm::vec4 colour   = renderable->colour();
    //const GLuint texture_id  = renderable->texture()->id();

    // add each vertex from this renderable into the buffer, advance the buffer
    // pointer each time so that it points at the next BRVertex

    //const auto colour = renderable->colour*255.0f;
    const uint32_t a = static_cast<uint32_t>(255.0f*renderable->colour.a);
    const uint32_t b = static_cast<uint32_t>(255.0f*renderable->colour.b);
    const uint32_t g = static_cast<uint32_t>(255.0f*renderable->colour.g);
    const uint32_t r = static_cast<uint32_t>(255.0f*renderable->colour.r);

    const uint32_t c = a << 24 | b << 16 | g << 8 | r;

    // bottom left
    vertex_map->position.x = renderable->position.x - 0.5*renderable->size.x;
    vertex_map->position.y = renderable->position.y - 0.5*renderable->size.y;
    vertex_map->position.z = renderable->position.z;
    vertex_map->uv.x       = 0.0f;
    vertex_map->uv.y       = 1.0f;
    //vertex_map->colour     = renderable->colour;
    vertex_map->colour     = c;
    vertex_map->texture_id = renderable->texture->id();
    ++vertex_map;

    // bottom right
    vertex_map->position.x = renderable->position.x + 0.5*renderable->size.x;
    vertex_map->position.y = renderable->position.y - 0.5*renderable->size.y;
    vertex_map->position.z = renderable->position.z;
    vertex_map->uv.x       = 1.0f;
    vertex_map->uv.y       = 1.0f;
    //vertex_map->colour     = renderable->colour;
    vertex_map->colour     = c;
    vertex_map->texture_id = renderable->texture->id();
    ++vertex_map;

    // top right
    vertex_map->position.x = renderable->position.x + 0.5*renderable->size.x;
    vertex_map->position.y = renderable->position.y + 0.5*renderable->size.y;
    vertex_map->position.z = renderable->position.z;
    vertex_map->uv.x       = 1.0f;
    vertex_map->uv.y       = 0.0f;
    //vertex_map->colour     = renderable->colour;
    vertex_map->colour     = c;
    vertex_map->texture_id = renderable->texture->id();
    ++vertex_map;

    // top left
    vertex_map->position.x = renderable->position.x - 0.5*renderable->size.x;
    vertex_map->position.y = renderable->position.y + 0.5*renderable->size.y;
    vertex_map->position.z = renderable->position.z;
    vertex_map->uv.x       = 0.0f;
    vertex_map->uv.y       = 0.0f;
    //vertex_map->colour     = renderable->colour;
    vertex_map->colour     = c;
    vertex_map->texture_id = renderable->texture->id();
    ++vertex_map;

    // for every 4 vertices we add to the buffer, we need 6 indices to make 2 triangles
    index_count += 6;
}

void BatchRenderer::flush()
{
    vao_.bind();
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (const void*)0);
    vao_.unbind();

    index_count = 0;
}
