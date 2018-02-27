#pragma once

#include <buffer.h>
#include <index_buffer.h>
#include <mesh_vertex.h>
#include <reflection.h>

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
                    //const std::vector<int> &offsets,
                    const std::vector<std::size_t> &offsets,
                    //const unsigned stride) const;
                    const std::size_t stride) const;

    void add_buffer(const Buffer &buffer,
                    const std::vector<GLuint> &indices,
                    const ReflectorBase &r)
    {
        // bind the vertex array
        bind();

        // get the number of vertex attributes to add to the array
        unsigned n_size = indices.size();

        // check it's the same as the number of sizes
        assert(n_size == r.sizes.size());

        // check it's the same as the number of offsets
        assert(n_size == r.offsets.size());

        // check it's the same as the number of offsets
        assert(n_size == r.components.size());

        // check it's the same as the number of offsets
        assert(n_size == r.gl_types.size());

        // bind the buffer we're sending in
        buffer.bind();

        // get the stride (the size of the object r belongs to)
        const std::size_t stride = r.size();

        // loop over the attributes
        for(unsigned i = 0; i < n_size; ++i)
        {
            // enable the appropriate vertex attribute and pass the data offset etc
            // into the array
            glEnableVertexAttribArray(indices[i]);
            if(r.gl_types[i] == GL_UNSIGNED_BYTE || r.gl_types[i] == GL_UNSIGNED_INT)
            {
                glVertexAttribPointer(indices[i], r.components[i], r.gl_types[i], GL_TRUE,
                                      r.size(), (const void*)r.offsets[i]);
            }
            else
            {
                glVertexAttribPointer(indices[i], r.components[i], r.gl_types[i], GL_FALSE,
                                      r.size(), (const void*)r.offsets[i]);
            }
        }

        // unbind the buffer
        buffer.unbind();

        // unbind the vertex array
        unbind();
    }

    // add a buffer of MeshVertex objects, with default assignment of attribute
    // numbers according to which data types are specified
    void add_buffer(const Buffer &buffer,
                    const std::vector<MeshVertexDataType> &data_types) const;

    // add a buffer of MeshVertex objects, with manual assignment of attribute
    // numbers according to corresponding elements of data_types and
    // attribute_numbers
    void add_buffer(const Buffer &buffer,
                    const std::vector<MeshVertexDataType> &data_types,
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
