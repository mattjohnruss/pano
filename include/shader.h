#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>

class Shader
{
public:
    Shader(const GLchar *vertex_path, const GLchar *fragment_path);
    ~Shader();

    void use() const;

    const GLuint program() const;

private:
    GLuint program_;
};

#endif // SHADER_H_
