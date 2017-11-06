#include "texture.h"

// STB_IMAGE_IMPLEMENTATION is defined in image.cpp for faster rebuilds
#include <stb_image.h>
#include <iostream>

Texture2D::Texture2D(const char *image_path)
{
    // attempt to load image located at image_path into image_data
    int image_width, image_height, n_components;
    unsigned char *image_data = stbi_load(image_path,
            &image_width, &image_height, &n_components, 0);

    // if the load was successful
    if(image_data != 0)
    {
        GLenum texture_format;

        // set the texture format based on the number of components in the loaded image
        switch(n_components)
        {
            case 1:
                texture_format = GL_RED;
                break;
            case 3:
                texture_format = GL_RGB;
                break;
            case 4:
                texture_format = GL_RGBA;
                break;
        }

        // generate a texture and store the id in id_
        glGenTextures(1, &id_);

        // bind the texture at id_
        glBindTexture(GL_TEXTURE_2D, id_);

        // TODO move these parameters into member functions
        // make the texture wrap in both directions
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set min and mag filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   

        // create the texture from the image data
        glTexImage2D(GL_TEXTURE_2D, 0, texture_format, image_width, image_height, 0,
                texture_format, GL_UNSIGNED_BYTE, image_data);

        // generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);


        // unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        std::cerr << "ERROR::TEXTURE::STBI_LOAD_FAILED\n";
        exit(1);
    }

    // free the image memory
    stbi_image_free(image_data);
}

Texture2D::~Texture2D()
{
}

void Texture2D::use(GLenum active_texture) const
{
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_2D, id_);
}

const GLuint Texture2D::id() const
{
    return id_;
}
