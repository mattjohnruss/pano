#pragma once

#include <glad/glad.h>
#include <vector>

// TODO destructor which deletes the gl buffer, unless we decide to give
// VertexArray ownership of the data

class IndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const std::vector<GLuint> &indices);

    void set_data(const std::vector<GLuint> &indices);

    void bind() const;
    void unbind() const;

    unsigned size() const;

private:
    GLuint id_;
    unsigned size_;
};
