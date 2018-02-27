#pragma once

#include <mesh_vertex.h>
#include <glad/glad.h>
#include <vector>

// TODO destructor which deletes the gl buffer

class Buffer
{
public:
    // Default constructor - just generate a buffer (unless flag is false)
    Buffer(const bool generate_buffer = true);

    // create a buffer from an existing gl buffer id
    // useful for being able to create a buffer in a unusual way (e.g.
    // non-float data etc) and then wrap our class around it
    Buffer(const GLuint id);

    // create a buffer and fill it with float data
    Buffer(const GLfloat *data,
           const unsigned n_element);

    // create a buffer and fill it with repeating float data
    Buffer(const GLfloat *data,
           const unsigned n_element,
           const unsigned n_repeat);

    // create a buffer and fill it with MeshVertex objects
    Buffer(const std::vector<MeshVertex> &vertices);

    // fill the buffer with float data
    void set_data(const GLfloat *data,
                  const unsigned n_element);

    // fill the buffer with repeating float data
    void set_data(const GLfloat *data,
                  const unsigned n_element,
                  const unsigned n_repeat);

    // fill the buffer with MeshVertex objects
    void set_data(const std::vector<MeshVertex> &vertices);

    // bind the buffer
    void bind() const;

    // unbind the buffer
    void unbind() const;

    // map the buffer as write only and return the pointer using the type given
    // as a template arg
    template <typename T>
    T* map_write_only() const
    {
        return static_cast<T*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    }

    // unmap the buffer
    void unmap() const;

    // return the id
    GLuint& id();

    // return the id (const)
    const GLuint& id() const;

private:
    // id of the underlying opengl buffer object
    GLuint id_;
};
