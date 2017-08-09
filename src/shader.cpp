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

    GLuint vertex;
    GLuint fragment;
    GLint success;
    GLchar infolog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infolog << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infolog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infolog << std::endl;
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
        std::cerr << "ERROR::PROGRAM::LINK_FAILED\n"
                  << infolog << std::endl;
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
    glDeleteProgram(program_);
}

void Shader::use() const
{
    glUseProgram(program_);
}

const GLuint Shader::program() const
{
    return program_;
}

