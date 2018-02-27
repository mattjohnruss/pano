#include <renderer.h>
#include <buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>
#include <renderable2d.h>
#include <reflection.h>

struct BRVertex : public ReflectiveStruct<BRVertex>
{
    glm::vec3 position;
    glm::vec2 uv;
    //glm::vec4 colour;
    uint32_t colour;
    float texture_id;

    static void setup()
    {
        register_field(BRVertex, position,   3, GL_FLOAT);
        register_field(BRVertex, uv,         2, GL_FLOAT);
        register_field(BRVertex, colour,     4, GL_UNSIGNED_BYTE);
        register_field(BRVertex, texture_id, 1, GL_FLOAT);
    }
};

class BatchRenderer : public Renderer
{
public:
    BatchRenderer();
    virtual ~BatchRenderer();

    void begin();
    void end();

    using Renderer::submit;
    inline void submit(const Renderable2D *const renderable) override;
    void flush() override; 

    static constexpr unsigned n_max_renderables = 62500;

private:
    Buffer vbo_;
    IndexBuffer ibo_;
    VertexArray vao_;

    BRVertex *vertex_map;
    unsigned index_count;
};
