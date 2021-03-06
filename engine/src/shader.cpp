#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar *vertex_path, const GLchar *fragment_path)
{
    std::string vertex_source_s;
    std::string fragment_source_s;

    std::ifstream vertex_file;
    std::ifstream fragment_file;

    vertex_file.open(vertex_path);
    fragment_file.open(fragment_path);

    std::stringstream vertex_sstream;
    std::stringstream fragment_sstream;

    vertex_sstream << vertex_file.rdbuf();
    fragment_sstream << fragment_file.rdbuf();

    vertex_file.close();
    fragment_file.close();

    vertex_source_s = vertex_sstream.str();
    fragment_source_s = fragment_sstream.str();

    const GLchar *vertex_source = vertex_source_s.c_str();
    const GLchar *fragment_source = fragment_source_s.c_str();

    GLint success;
    const unsigned infolog_size = 512;
    GLchar infolog[infolog_size];

    // vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertex, infolog_size, NULL, infolog);
        std::cerr << "Shader::Shader - vertex shader compilation failed\n"
                  << "(in file \"" << vertex_path << "\")\n"
                  << infolog << '\n';
    }

    // fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragment, infolog_size, NULL, infolog);
        std::cerr << "Shader::Shader - fragment shader compilation failed\n"
                  << "(in file \"" << fragment_path << "\")\n"
                  << infolog << '\n';
    }

    // program
    program_ = glCreateProgram();
    glAttachShader(program_, vertex);
    glAttachShader(program_, fragment);
    glLinkProgram(program_);

    glGetProgramiv(program_, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(program_, 512, NULL, infolog);
        std::cerr << "Shader::Shader - shader program link failed\n"
                  << infolog << '\n';
    }

    // detach the vertex and fragment shaders from the program
    glDetachShader(program_, vertex);
    glDetachShader(program_, fragment);

    // delete the shader objects
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    // delete the shader program
    glDeleteProgram(program_);
}

void Shader::use() const
{
    // use the shader program
    glUseProgram(program_);
}

const GLuint Shader::program() const
{
    // return the id of the shader program
    return program_;
}

// Scalars
void Shader::set_bool(const char *name, const GLboolean &value) const
{
    glUniform1i(glGetUniformLocation(program_, name), (GLint)value);
}

void Shader::set_int(const char *name, const GLint &value) const
{
    glUniform1i(glGetUniformLocation(program_, name), value);
}

void Shader::set_uint(const char *name, const GLuint &value) const
{
    glUniform1ui(glGetUniformLocation(program_, name), value);
}

void Shader::set_float(const char *name, const GLfloat &value) const
{
    glUniform1f(glGetUniformLocation(program_, name), value);
}

// Vectors
void Shader::set_vec2(const char *name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(program_, name), 1, &value[0]);
}

void Shader::set_vec2(const char *name, const GLfloat &x) const
{
    glUniform2f(glGetUniformLocation(program_, name), x, x);
}

void Shader::set_vec2(const char *name, const GLfloat &x, const GLfloat &y) const
{
    glUniform2f(glGetUniformLocation(program_, name), x, y);
}

void Shader::set_vec3(const char *name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(program_, name), 1, &value[0]);
}

void Shader::set_vec3(const char *name, const GLfloat &x) const
{
    glUniform3f(glGetUniformLocation(program_, name), x, x, x);
}

void Shader::set_vec3(const char *name, const GLfloat &x, const GLfloat &y, const GLfloat &z) const
{
    glUniform3f(glGetUniformLocation(program_, name), x, y, z);
}

void Shader::set_vec4(const char *name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(program_, name), 1, &value[0]);
}

void Shader::set_vec4(const char *name, const GLfloat &x) const
{
    glUniform4f(glGetUniformLocation(program_, name), x, x, x, x);
}

void Shader::set_vec4(const char *name, const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &w)
{
    glUniform4f(glGetUniformLocation(program_, name), x, y, z, w);
}

// Matrices
void Shader::set_mat2(const char *name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(program_, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat3(const char *name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(program_, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat4(const char *name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_, name), 1, GL_FALSE, &mat[0][0]);
}
