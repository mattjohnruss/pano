#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

// TODO keep track of which texture units are active and automatically use next
// available one in use() (static member of Texture or singleton
// TextureManager?)

// maybe we want a stack?
// pop a texture unit off the stack when activated; push back on when deactivated.
// std::stack tex_unit_stack;
//
// ideally we will make Mesh (and Model?) use the full Texture class instead of
// the minimal MeshTexture
//
// for(unsigned i = 0; i < n_tex_units; ++i)
// {
//     tex_unit_stack.push(<tex_unit_free_value>);
// }

class Texture
{
public:
    Texture(GLuint id);
    virtual ~Texture();

    virtual void use(GLenum active_texture) const = 0;

    const GLuint id() const;

protected:
    GLuint id_;
};

class Texture2D : public Texture
{
public:
    Texture2D(const char *image_path);
    ~Texture2D();

    void use(GLenum active_texture) const;

    static GLuint texture_from_file(const char *image_path, const std::string &directory);
};

class Texture3D : public Texture
{
public:
    Texture3D(const std::vector<std::string> &image_paths);
    ~Texture3D();

    void use(GLenum active_texture) const;

    static GLuint texture_from_file(
            const std::vector<std::string> &image_paths, const std::string &directory);
};
