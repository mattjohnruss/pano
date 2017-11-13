#pragma once

#include <glad/glad.h>

#include <string>

class Texture2D
{
public:
    Texture2D(const char *image_path);
    ~Texture2D();

    void use(GLenum active_texture) const;

    const GLuint id() const;

    static GLuint texture_from_file(const char *image_path, const std::string &directory);

private:
    GLuint id_;
};
