#include <index_buffer.h>

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &id_);
}

IndexBuffer::IndexBuffer(const std::vector<GLuint> &indices)
{
    glGenBuffers(1, &id_);
    set_data(indices);
}

void IndexBuffer::set_data(const std::vector<GLuint> &indices)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size()*sizeof(GLuint),
                 &indices[0],
                 GL_STATIC_DRAW);
    unbind();
    size_ = indices.size();
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned IndexBuffer::size() const
{
    return size_;
}
