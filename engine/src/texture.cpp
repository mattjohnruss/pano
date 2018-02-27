#include <texture.h>

// STB_IMAGE_IMPLEMENTATION is defined in image.cpp for faster rebuilds
#include <stb_image.h>

#include <iostream>
#include <algorithm>

Texture::Texture(GLuint id)
    : id_(id)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &id_);
}

const GLuint Texture::id() const
{
    return id_;
}

Texture2D::Texture2D(const std::string &image_path)
    : Texture(texture_from_file(image_path, ""))
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::use(GLenum active_texture) const
{
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_2D, id_);
}

GLuint Texture2D::texture_from_file(const std::string &image_path,
                                    const std::string &directory)
{
    std::string filename;

    if(directory == "")
    {
        filename = image_path;
    }
    else
    {
        filename = directory + "/" + image_path;
    }

    // replace all occurences of Windows style '\' with '/' in texture path
    std::replace(filename.begin(), filename.end(), '\\', '/');

#ifdef PANO_DEBUG
    // output the (parsed) texture path
    std::cout << "Texture2D::texture_from_file - filename = " << filename << '\n';
#endif

    // declare the texture handle
    GLuint id;

    // generate a texture and store the id
    glGenTextures(1, &id);

    // image dimensions and number of components
    int image_width;
    int image_height;
    int n_components;

    // attempt to load image located at image_path into image_data
    unsigned char *image_data =
        stbi_load(filename.c_str(), &image_width, &image_height, &n_components, 0);

    // if the load was successful
    if(image_data != 0)
    {
        // texture format
        GLenum texture_format;

        // bind the texture at id
        glBindTexture(GL_TEXTURE_2D, id);

        // set the texture format based on the number of components in the loaded image
        switch(n_components)
        {
            case 1:
                texture_format = GL_RED;
                {
                    GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
                }
                break;
            case 2:
                texture_format = GL_RG;
                {
                    GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
                    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
                }
                break;
            case 3:
                texture_format = GL_RGB;
                break;
            case 4:
                texture_format = GL_RGBA;
                break;
            default:
                std::cerr << "Texture2D::texture_from_file - n_components = "
                          << n_components << " unsupported\n";
                std::cerr << "Attempting to use texture_format = GL_RGB\n";
                texture_format = GL_RGB;
        }

        // TODO move these parameters into member functions
        // make the texture wrap in both directions
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set min and mag filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
        std::cerr << "Texture2D::texture_from_file - stbi_load failed\n";
        //exit(1);
    }

    // free the image memory
    stbi_image_free(image_data);

    // return the texture id
    return id;
}

Texture3D::Texture3D(const std::vector<std::string> &image_paths)
    : Texture(texture_from_file(image_paths, ""))
{
    std::cout << "Texture3D::Texture3D - id_ = " << id_ << '\n';
}

Texture3D::~Texture3D()
{
}

void Texture3D::use(GLenum active_texture) const
{
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_3D, id_);
}

GLuint Texture3D::texture_from_file(
        const std::vector<std::string> &image_paths, const std::string &directory)
{
    // declare the texture handle
    GLuint id;

    // generate a texture and store the id
    glGenTextures(1, &id);

    // get the number of images
    unsigned n_image = image_paths.size();

    // check if the image list is empty
    if(n_image == 0)
    {
        std::cerr << "Texture3D::texture_from_file - vector of image paths is empty\n";

        // TODO is it ok to return a texture handle of 0?
        // trying to load 0 textures is a bug anyway so yolo
        return 0;
    }

    // vector of processed filenames
    std::vector<std::string> filenames(n_image);

    // bind the texture at id
    glBindTexture(GL_TEXTURE_3D, id);

    // loop over the images
    for(unsigned i = 0; i < n_image; ++i)
    {
        if(directory == "")
        {
            filenames[i] = image_paths[i];
        }
        else
        {
            filenames[i] = directory + "/" + image_paths[i];
        }

        // replace all occurences of Windows style '\' with '/' in texture path
        std::replace(filenames[i].begin(), filenames[i].end(), '\\', '/');

        // output the (parsed) texture path
        std::cout << "Texture3D::texture_from_file - "
                  << "filenames[" << i << "] = " << filenames[i] << '\n';

        // image dimensions and number of components
        int image_width;
        int image_height;
        int n_components;

        // texture format
        GLenum texture_format;

        // attempt to load image located at image_path into image_data
        unsigned char *image_data =
            stbi_load(filenames[i].c_str(), &image_width, &image_height, &n_components, 0);

        // if the load was successful
        if(image_data != 0)
        {
            // check the number of components in the first image only and allocate gl storage
            // allows us to loop only once over textures and not store all image data at once
            if(i == 0)
            {
                // set the texture format based on the number of components in the loaded image
                switch(n_components)
                {
                    case 1:
                            texture_format = GL_RED;
                        {
                            GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
                            glTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
                        }
                        break;
                    case 2:
                            texture_format = GL_RG;
                        {
                            GLint swizzle_mask[] = {GL_RED, GL_RED, GL_RED, GL_GREEN};
                            glTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);
                        }
                        break;
                    case 3:
                        texture_format = GL_RGB;
                        break;
                    case 4:
                        texture_format = GL_RGBA;
                        break;
                    default:
                        std::cerr << "Texture3D::texture_from_file - n_components = " << n_components << "unsupported\n";
                        std::cerr << "Attempting to use texture_format = GL_RGB\n";
                        texture_format = GL_RGB;
                }

                // create the texture from the image data
                glTexImage3D(GL_TEXTURE_3D, 0, texture_format, image_width, image_height,
                        static_cast<GLsizei>(n_image), 0, texture_format, GL_UNSIGNED_BYTE, NULL);

                // TODO move these parameters into member functions
                // make the texture wrap in s,t directions and clamp to edge in r
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                // set min and mag filters
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }

            // load image data from current image into texture
            glTexSubImage3D(
                    GL_TEXTURE_3D, 0,
                    0, 0, i,
                    image_width, image_height, 1,
                    texture_format, GL_UNSIGNED_BYTE, image_data);
        }
        else
        {
            std::cerr << "Texture3D::texture_from_file - stbi_load failed\n";
            //exit(1);
        }

        // free the image memory
        stbi_image_free(image_data);
    }

    // generate mipmaps
    glGenerateMipmap(GL_TEXTURE_3D);

    // unbind the texture
    glBindTexture(GL_TEXTURE_3D, 0);

    // return the texture id
    return id;
}
