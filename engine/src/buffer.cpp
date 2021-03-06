#include <buffer.h>

Buffer::Buffer(const bool generate_buffer)
{
    if(generate_buffer)
    {
       glGenBuffers(1, &id_);
    }
}

Buffer::Buffer(const GLuint id)
    : id_(id)
{
}

Buffer::Buffer(const GLfloat *data,
               const unsigned n_element)
{
    glGenBuffers(1, &id_);
    set_data(data, n_element);
}

Buffer::Buffer(const std::vector<MeshVertex> &vertices)
{
    glGenBuffers(1, &id_);
    set_data(vertices);
}

Buffer::Buffer(const GLfloat *data, const unsigned n_element, const unsigned n_repeat)
{
    set_data(data, n_element, n_repeat);
}


void Buffer::set_data(const GLfloat *data,
                      const unsigned n_element)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER,
                 n_element*sizeof(GLfloat),
                 &data[0],
                 GL_STATIC_DRAW);
    unbind();
}

void Buffer::set_data(const GLfloat *data,
                      const unsigned n_element,
                      const unsigned n_repeat)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER,
                 n_element*n_repeat*sizeof(GLfloat),
                 NULL,
                 GL_STATIC_DRAW);

    const unsigned size = n_element*sizeof(GLfloat);

    for(unsigned i = 0; i < n_repeat; ++i)
    {
        const unsigned offset = i*size;

        glBufferSubData(GL_ARRAY_BUFFER,
                        offset,
                        size,
                        &data[0]);
    }
}

void Buffer::set_data(const std::vector<MeshVertex> &vertices)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size()*sizeof(MeshVertex),
                 &vertices[0],
                 GL_STATIC_DRAW);
    unbind();
}

void Buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id_);
}

void Buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffer::unmap() const
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

GLuint& Buffer::id()
{
    return id_;
}

const GLuint& Buffer::id() const
{
    return id_;
}
