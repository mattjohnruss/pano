#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB_IMAGE_IMPLEMENTATION is defined in image.cpp for faster rebuilds
#include <stb_image.h>

#include <iostream>
#include <cmath>

#include <shader.h>
#include <camera.h>
#include <graph.h>

int main()
{
    using glm::radians;

    const GLuint window_width = 800;
    const GLuint window_height = 600;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // create a window
    GLFWwindow* window =
        glfwCreateWindow(window_width, window_height, "Pano", NULL, NULL);

    glfwMakeContextCurrent(window);

    if(window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // set the opengl function pointers using glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return 1;
    }

    // define the viewport dimensions
    glViewport(0, 0, window_width, window_height);

    Pano::Graph g;
    g.add_node();
    g.add_node();
    g.add_node();

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
