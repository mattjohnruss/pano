#include <vertex_array.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id_);
}

// TODO decide which class should manage the buffer memory. VertexArray could
// assume ownership of the buffers it handles, since the *Buffer objects can be
// destroyed after being added to the VAO as attributes, provided they do not
// call glDeleteBuffers (probably). If VertexArray should own the data, we need
// to delete it in the destructor below, otherwise *Buffer objects need to be
// kept alive as long as we need their data, and they will delete their own
// data. Note that we will have to allow access to (and keep a pointer to) the
// underlying buffer storage so we can still delete it after the Buffer objects
// have been deleted.
VertexArray::~VertexArray()
{
    //const unsigned n_buffer = buffers_.size;

    //for(unsigned i = 0; i < n_buffer; ++i)
    //{
        //delete buffers_[i];
    //}
}

void VertexArray::add_buffer(const Buffer &buffer,
                             const GLuint index,
                             const GLint size) const
{
    bind();
    buffer.bind();

    glEnableVertexAttribArray(index);

    glVertexAttribPointer(
                index, size, GL_FLOAT, GL_FALSE,
                size*sizeof(GLfloat), (void*)0);

    buffer.unbind();
    unbind();
}

void VertexArray::add_buffer(const Buffer &buffer,
                             const std::vector<GLuint> &indices,
                             const std::vector<GLint> &sizes,
                             const std::vector<int> &offsets,
                             const unsigned stride) const
{
    // bind the vertex array
    bind();

    // get the number of vertex attributes to add to the array
    unsigned n_size = indices.size();

    // check it's the same as the number of sizes
    assert(n_size == sizes.size());

    // check it's the same as the number of offsets
    assert(n_size == offsets.size());

    // bind the buffer we're sending in
    buffer.bind();

    // loop over the attributes
    for(unsigned i = 0; i < n_size; ++i)
    {
        GLuint index = indices[i];
        GLint size = sizes[i];
        unsigned offset = offsets[i];

        // enable the appropriate vertex attribute and pass the data offset etc
        // into the array
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
                    index, size, GL_FLOAT, GL_FALSE,
                    stride*sizeof(GLfloat), (void*)(offset*sizeof(GLfloat)));
    }

    // unbind the buffer
    buffer.unbind();

    // unbind the vertex array
    unbind();
}

void VertexArray::add_buffer(const Buffer &buffer,
                             const std::vector<VertexDataType> &data_types) const
{
    std::vector<GLuint> indices(data_types.size());

    for(unsigned i = 0; i < data_types.size(); ++i)
    {
        indices[i] = static_cast<GLuint>(i);
    }

    add_buffer(buffer, data_types, indices);
}

void VertexArray::add_buffer(const Buffer &buffer,
                             const std::vector<VertexDataType> &data_types,
                             const std::vector<GLuint> &indices) const
{
    // bind the vertex array
    bind();

    // get the number of vertex attributes to add to the array
    unsigned n_index = indices.size();

    // check it's the same as the number of data types
    assert(n_index == data_types.size());

    // bind the buffer we're sending in
    buffer.bind();

    // loop over the attributes
    for(unsigned i = 0; i < n_index; ++i)
    {
        GLuint index = indices[i];
        VertexDataType data_type = data_types[i];

        // enable the appropriate vertex attribute and pass the data offset etc
        // into the array
        switch(data_type)
        {
            case VertexDataType::POSITION:
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(
                        index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, position));
                break;

            case VertexDataType::NORMAL:
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(
                        index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, normal));
                break;

            case VertexDataType::TEX_COORDS:
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(
                        index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, tex_coords));
                break;

            case VertexDataType::COLOUR:
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(
                        index, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, colour));
                break;
        }
    }

    // unbind the buffer
    buffer.unbind();

    // unbind the vertex array
    unbind();
}

void VertexArray::add_index_buffer(const IndexBuffer &index_buffer) const
{
    // bind the vertex array
    bind();

    // bind the buffer we're sending in
    index_buffer.bind();

    // unbind the vertex array
    unbind();

    // unbind the buffer after unbinding the array since for index buffers it
    // is the binding that the vao remembers rather than the attributes of a
    // vertex buffer. if we unbound the index buffer first the vao would forget
    // about it
    index_buffer.unbind();
}

void VertexArray::bind() const
{
    glBindVertexArray(id_);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}
