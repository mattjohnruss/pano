#pragma once

#include <buffer.h>
#include <index_buffer.h>
#include <vertex.h>

class VertexArray
{
public:
    // default constructor - only creates a gl vertex array object
    VertexArray();

    // destructor - will (possibly) end up deleting the buffer data if we
    // decide to let VertexArrays take ownership of it rather than letting the
    // Buffer retain ownership
    ~VertexArray();

    // add a buffer using the specified attribute number
    // can use this to add multiple attributes which each point to different
    // buffers
    void add_buffer(const Buffer &buffer,
                    const GLuint index,
                    const GLint size) const;

    // add a buffer using the specified list of attribute numbers (with
    // associated sizes and offsets)
    // can use this to add multiple attributes which each point to different
    // parts of the same buffer
    void add_buffer(const Buffer &buffer,
                    const std::vector<GLuint> &indices,
                    const std::vector<GLint> &sizes,
                    const std::vector<int> &offsets,
                    const unsigned stride) const;

    // add a buffer of Vertex objects, with default assignment of attribute
    // numbers according to which data types are specified
    void add_buffer(const Buffer &buffer,
                    const std::vector<VertexDataType> &data_types) const;

    // add a buffer of Vertex objects, with manual assignment of attribute
    // numbers according to corresponding elements of data_types and
    // attribute_numbers
    void add_buffer(const Buffer &buffer,
                    const std::vector<VertexDataType> &data_types,
                    const std::vector<GLuint> &indices) const;

    // add an index buffer
    void add_index_buffer(const IndexBuffer &index_buffer) const;

    // bind the VertexArray
    void bind() const;

    // unbind the VertexArray
    void unbind() const;

private:
    // the id of the underlying opengl vertex array object
    GLuint id_;

    //std::vector<Buffer*> buffers_;
};
