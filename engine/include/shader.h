#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
    // construct a shader from vert and frag source code on disk
    Shader(const GLchar *vertex_path, const GLchar *fragment_path);

    // delete the shader program
    ~Shader();

    // use the shader program
    void use() const;

    // return the id of the shader program
    const GLuint program() const;

    // Scalars
    void set_bool(const char *name, const GLboolean &value) const;
    void set_int(const char *name, const GLint &value) const;
    void set_uint(const char *name, const GLuint &value) const;
    void set_float(const char *name, const GLfloat &value) const;

    // Vectors
    void set_vec2(const char *name, const glm::vec2 &value) const;
    void set_vec2(const char *name, const GLfloat &x) const;
    void set_vec2(const char *name, const GLfloat &x, const GLfloat &y) const;
    void set_vec3(const char *name, const glm::vec3 &value) const;
    void set_vec3(const char *name, const GLfloat &x) const;
    void set_vec3(const char *name, const GLfloat &x, const GLfloat &y, const GLfloat &z) const;
    void set_vec4(const char *name, const glm::vec4 &value) const;
    void set_vec4(const char *name, const GLfloat &x) const;
    void set_vec4(const char *name, const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &w);

    // Matrices
    void set_mat2(const char *name, const glm::mat2 &mat) const;
    void set_mat3(const char *name, const glm::mat3 &mat) const;
    void set_mat4(const char *name, const glm::mat4 &mat) const;

private:
    GLuint program_;
};
