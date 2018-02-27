//#include <mesh.h>
#include <model.h>
#include <texture.h>
#include <shader.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Mesh::Mesh(
        const aiMesh *const mesh,
        const aiScene *const scene,
        const std::string &directory,
        Model &parent_model) :
    directory_(directory)
{
    // reserve space in the vertices vector
    vertices_.reserve(mesh->mNumVertices);

    // loop over the vertices
    for(unsigned i = 0; i < mesh->mNumVertices; ++i)
    {
        MeshVertex vertex;

        // copy the positions from the mesh
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        // copy the normals from the mesh
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        // copy the texture coords from the mesh
        if(mesh->mTextureCoords[0])
        {
            vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        // add the vertex to the vector
        vertices_.push_back(vertex);
    }

    // loop over the faces
    for(unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace &face = mesh->mFaces[i];

        // loop over the indices
        for(unsigned j = 0; j < face.mNumIndices; ++j)
        {
            indices_.push_back(face.mIndices[j]);
        }
    }

    // test if the mesh contains any materials
    if(mesh->mMaterialIndex >= 0)
    {
        // get the material for the current mesh
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // get the diffuse maps
        load_material_textures(material, aiTextureType_DIFFUSE, parent_model);

        // get the specular maps
        load_material_textures(material, aiTextureType_SPECULAR, parent_model);
    }

    // setup and populate the gl buffers with the mesh data
    vbo_.set_data(vertices_);
    ebo_.set_data(indices_);
    vao_.add_buffer(vbo_, {MeshVertexDataType::POSITION, MeshVertexDataType::NORMAL, MeshVertexDataType::TEX_COORDS}, {0,1,2});
    vao_.add_index_buffer(ebo_);
}

void Mesh::draw(const Shader &shader, const GLenum mode) const
{
    // counters for the diffuse and specular textures
    unsigned i_diffuse = 0;
    unsigned i_specular = 0;

    // get the number of textures in the mesh
    unsigned n_texture = textures_.size();

    // loop over the textures
    // TODO does Assimp split the model so that each mesh only has one texture
    // of each type? At least .obj files seem to work like that. Maybe let's
    // stick to one model file type to make writing shaders and setting
    // uniforms for Mesh drawing simpler.
    for(unsigned i = 0; i < n_texture; ++i)
    {
        // is adding to GL_TEXTURE0 allowed? part of gl spec says not
        // necessarily contiguous but other part suggests it's ok...
        // seems to work for now
        glActiveTexture(GL_TEXTURE0 + i);

        // storage for the current texture number as a string
        std::string number;

        // check the type of the texture
        if(textures_[i]->type == aiTextureType_DIFFUSE)
        {
            // if it's diffuse, increment diffuse counter and convert to a string
            // TODO optimise this by not constructing multiple std::string
            // objects per frame per texture per mesh!
            ++i_diffuse;
            number = std::to_string(i_diffuse);

            // set the uniform for the texture
            shader.set_int(("material.texture_diffuse" + number).c_str(), i);
        }
        else if(textures_[i]->type == aiTextureType_SPECULAR)
        {
            // if it's specular, increment specular counter and convert to a string
            ++i_specular;
            number = std::to_string(i_specular);

            // set the uniform for the texture
            shader.set_int(("material.texture_specular" + number).c_str(), i);
        }

        // bind the texture
        glBindTexture(GL_TEXTURE_2D, textures_[i]->id);
    }

    // reset the active texture the 0 (is there much point in this?)
    glActiveTexture(GL_TEXTURE0);

    // draw the mesh
    //glBindVertexArray(VAO_);
    vao_.bind();
    glDrawElements(mode, indices_.size(), GL_UNSIGNED_INT, 0);
    vao_.unbind();
    //glBindVertexArray(0);
}

void Mesh::load_material_textures(
        aiMaterial *material,
        aiTextureType tex_type,
        Model &parent_model)
{
    // get the number of textures in the material of the specified type
    unsigned n_texture = material->GetTextureCount(tex_type);

    //std::cout << "n_texture (" << (tex_type == aiTextureType_DIFFUSE ? "diffuse" : "specular") << ") = " << n_texture << '\n';

    // loop over the textures
    for(unsigned i = 0; i < n_texture; ++i)
    {
        // get the path of the current texture
        aiString tex_path;
        material->GetTexture(tex_type, i, &tex_path);
        std::string std_tex_path(tex_path.C_Str());

        // get an iterator to the current texture in the model's global storage
        auto tex_it = parent_model.textures().find(std_tex_path);

        // check if we've already loaded this texture
        if(tex_it == parent_model.textures().end())
        {
            // if not, load it and store in map
            MeshTexture texture;
            texture.id = Texture2D::texture_from_file(tex_path.C_Str(), directory_);
            texture.type = tex_type;
            parent_model.textures()[std_tex_path] = texture;
        }

        // add a pointer to the MeshTexture object to the Mesh's local texture storage
        textures_.push_back(&parent_model.textures()[std_tex_path]);
    }
}
