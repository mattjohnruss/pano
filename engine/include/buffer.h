#pragma once

#include <vertex.h>
#include <glad/glad.h>
#include <vector>

// TODO destructor which deletes the gl buffer

class Buffer
{
public:
    Buffer();

    Buffer(const GLfloat *data,
           const unsigned n_element);

    Buffer(const GLfloat *data,
           const unsigned n_element,
           const unsigned n_repeat);

    Buffer(const std::vector<Vertex> &vertices);

    void set_data(const GLfloat *data,
                  const unsigned n_element);

    void set_data(const GLfloat *data,
                  const unsigned n_element,
                  const unsigned n_repeat);

    void set_data(const std::vector<Vertex> &vertices);

    void bind() const;
    void unbind() const;

private:
    // id of the underlying opengl buffer object
    GLuint id_;
};
