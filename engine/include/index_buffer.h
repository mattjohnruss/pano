#pragma once

#include <glad/glad.h>
#include <vector>

// TODO destructor which deletes the gl buffer, unless we decide to give
// VertexArray ownership of the data

class IndexBuffer
{
public:
    // Default constructor - just generate a buffer (unless flag is false)
    IndexBuffer(const bool generate_buffer = true);

    // generate a buffer and fill it with indices
    IndexBuffer(const std::vector<GLuint> &indices);

    // fill the buffer with indices
    void set_data(const std::vector<GLuint> &indices);

    // bind the buffer
    void bind() const;

    // unbind the buffer
    void unbind() const;

    // get the number of indices in the buffer
    unsigned size() const;

private:
    GLuint id_;
    unsigned size_;
};
