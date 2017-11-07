#include <shader.h>
#include <camera.h>
#include <graph.h>
#include <texture.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

// callback function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow *window, double x_offset, double y_offset);

namespace Movement
{
    // camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // mouse
    GLfloat previous_x = 0.0f;
    GLfloat previous_y = 0.0f;

    bool first_mouse = true;

    // timing
    GLfloat delta_time     = 0.0f;
    GLfloat current_frame  = 0.0f;
    GLfloat previous_frame = 0.0f;

    void do_movement();
}

// array of the keys currently pressed
namespace Keys
{
    bool pressed[1024];
}

int main()
{
    using glm::radians;

    // parameters and vertex data
    ////////////////////////////////////////////////////////////////////////////

    // window size
    const GLuint window_width = 1600;
    const GLuint window_height = 900;

    // vertices and normals of the cube
    GLfloat vertices[] =
    {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // positions of the various cubes
    glm::vec3 cube_positions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // point light positions
    glm::vec3 point_light_positions[] =
    {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // set up context
    ////////////////////////////////////////////////////////////////////////////

    glfwInit();

    // request opengl 3.3 core profile and disable window resizing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // create a window
    GLFWwindow* window =
        glfwCreateWindow(window_width, window_height, "Pano", NULL, NULL);

    // check the window was created successfully and quit if not
    if(window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // make it current
    glfwMakeContextCurrent(window);

    // set the callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // enable mouse capturing
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set the opengl function pointers using glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return 1;
    }

    // define the viewport dimensions
    glViewport(0, 0, window_width, window_height);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // test: graph object with some nodes
    //Pano::Graph g;
    //g.add_node();
    //g.add_node();
    //g.add_node();

    // VAO/VBO
    ////////////////////////////////////////////////////////////////////////////

    // Cube - declare handle, generate 1 VAO and assign ID to handle
    GLuint cube_VAO;
    glGenVertexArrays(1, &cube_VAO);

    // declare our VBO handle and generate one buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // bind the VAO
    glBindVertexArray(cube_VAO);

    // bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // allocate and fill data in the bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set the vertex attributes pointers for positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // set the vertex attributes pointers for normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // set the vertex attributes pointers for diffuse map
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // lamp - declare handle, generate 1 VAO and assign ID to handle
    GLuint lamp_VAO;
    glGenVertexArrays(1, &lamp_VAO);

    // bind the VAO
    glBindVertexArray(lamp_VAO);

    // allocate and fill data in the bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set the vertex attributes pointers for positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // shaders
    ////////////////////////////////////////////////////////////////////////////

    Shader lighting_shader("data/shaders/lighting.vert", "data/shaders/lighting.frag");
    Shader lamp_shader("data/shaders/lamp.vert", "data/shaders/lamp.frag");

    // textures
    ////////////////////////////////////////////////////////////////////////////

    Texture2D diffuse_map("data/textures/container2.png");
    Texture2D specular_map("data/textures/container2_specular.png");

    // coord transforms
    ////////////////////////////////////////////////////////////////////////////

    // view matrix
    glm::mat4 view_mat;

    // projection matrix
    glm::mat4 proj_mat;

    // game loop
    ////////////////////////////////////////////////////////////////////////////
    while(!glfwWindowShouldClose(window))
    {
        // update time of current frame
        Movement::current_frame  = glfwGetTime();
        Movement::delta_time     = Movement::current_frame - Movement::previous_frame;
        Movement::previous_frame = Movement::current_frame;

        // perform movement
        Movement::do_movement();

        // clear the background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // calculate the projection matrix
        proj_mat = glm::perspective(radians(Movement::camera.fov()),
                static_cast<float>(window_width)/static_cast<float>(window_height),
                0.1f, 100.0f);

        // get the view matrix from the camera
        Movement::camera.view_mat(view_mat);

        // idenitiy model matrix
        glm::mat4 model_mat(1.0f);

        // use the light shader and set uniforms
        lighting_shader.use();

        // directional light
        // -----------------
        lighting_shader.set_vec3("directional_light.direction", -0.2f, -1.0f, -0.3f);
        lighting_shader.set_vec3("directional_light.ambient", 0.05f);
        lighting_shader.set_vec3("directional_light.diffuse", 0.2f);
        lighting_shader.set_vec3("directional_light.specular", 0.25f);

        // point lights
        // ------------
        for(unsigned i = 0; i < 4; ++i)
        {
            std::string i_str = std::to_string(i);

            lighting_shader.set_vec3(("point_light[" + i_str + "].position").c_str(), point_light_positions[i]);
            lighting_shader.set_vec3(("point_light[" + i_str + "].ambient").c_str(), 0.05f);
            lighting_shader.set_vec3(("point_light[" + i_str + "].diffuse").c_str(), 0.8f);
            lighting_shader.set_vec3(("point_light[" + i_str + "].specular").c_str(), 1.0f);
            lighting_shader.set_float(("point_light[" + i_str + "].k_c").c_str(), 1.0f);
            lighting_shader.set_float(("point_light[" + i_str + "].k_l").c_str(), 0.09f);
            lighting_shader.set_float(("point_light[" + i_str + "].k_q").c_str(), 0.032f);
        }

        // spotlight
        // --------------
        lighting_shader.set_vec3("spotlight.position", Movement::camera.position());
        lighting_shader.set_vec3("spotlight.direction", Movement::camera.front());
        lighting_shader.set_float("spotlight.cos_cutoff", glm::cos(radians(15.0f)));
        lighting_shader.set_float("spotlight.cos_outer_cutoff", glm::cos(radians(15.5f)));
        lighting_shader.set_vec3("spotlight.ambient", 0.0f);
        lighting_shader.set_vec3("spotlight.diffuse", 1.0f);
        lighting_shader.set_vec3("spotlight.specular", 1.0f);
        lighting_shader.set_float("spotlight.k_c", 1.0f);
        lighting_shader.set_float("spotlight.k_l", 0.09f);
        lighting_shader.set_float("spotlight.k_q", 0.032f);

        // transformation matrices
        // -----------------------
        lighting_shader.set_mat4("proj_mat", proj_mat);
        lighting_shader.set_mat4("view_mat", view_mat);
        lighting_shader.set_vec3("view_position", Movement::camera.position());

        // material properties
        // -------------------
        lighting_shader.set_float("material.shininess", 32.0f);
        lighting_shader.set_int("material.diffuse", 0);
        lighting_shader.set_int("material.specular", 1);

        // bind the diffuse and specular map textures with the appropriate locations
        diffuse_map.use(GL_TEXTURE0);
        specular_map.use(GL_TEXTURE1);

        // render cubes
        glBindVertexArray(cube_VAO);

        // loop over cubes
        for(unsigned c = 0; c < 10; ++c)
        {
            // set the model matrix corresponding to the current cube's position
            model_mat = glm::mat4(1.0f);
            model_mat = glm::translate(model_mat, cube_positions[c]);

            // rotate the current cube a bit, update the model matrix and send as uniform
            float angle = c*20.0f;
            model_mat = glm::rotate(model_mat, radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            // set the model matrix uniform for the current cube
            lighting_shader.set_mat4("model_mat", model_mat);

            // calculate the normal matrix and send as a uniform
            glm::mat3 normal_mat(glm::transpose(glm::inverse(model_mat)));
            lighting_shader.set_mat3("normal_mat", normal_mat);

            // draw the current cube
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // draw point lights as lamps
        // --------------------------

        // use the lamp shader
        lamp_shader.use();

        // set the projection and view matrix uniforms
        lamp_shader.set_mat4("proj_mat", proj_mat);
        lamp_shader.set_mat4("view_mat", view_mat);

        // loop over the point lights
        for(unsigned i = 0; i < 4; ++i)
        {
            // set the model matrix using the lamp's position and scale
            model_mat = glm::mat4(1.0f);
            model_mat = glm::translate(model_mat, point_light_positions[i]);
            model_mat = glm::scale(model_mat, glm::vec3(0.1f));

            // set the model matrix uniform
            lamp_shader.set_mat4("model_mat", model_mat);

            // render lamp
            glBindVertexArray(lamp_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // unbind VAO
        glBindVertexArray(0);

        // swap buffers
        glfwSwapBuffers(window);

        // poll for events
        glfwPollEvents();
    }

    // delete arrays/buffers
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &lamp_VAO);
    glDeleteBuffers(1, &VBO);

    // terminate glfw
    glfwTerminate();

    return 0;
}

// GLFW key callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    using Keys::pressed;

    if(action == GLFW_PRESS)
    {
        if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
            glfwSetWindowShouldClose(window, GL_TRUE);
        //else if(key == GLFW_KEY_EQUAL)
            //Movement::mouse_sensitivity += 0.01f;
        //else if(key == GLFW_KEY_MINUS)
            //Movement::mouse_sensitivity -= 0.01f;

        if(key >= 0 && key < 1024)
            pressed[key] = true;
    }
    else if(action == GLFW_RELEASE && key >= 0 && key < 1024)
        pressed[key] = false;
}

// GLFW cursor position callback
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    using namespace Movement;
    using glm::radians;

    if(first_mouse)
    {
        previous_x = x_pos;
        previous_y = y_pos;
        first_mouse = false;
    }

    double x_offset = x_pos - previous_x;
    double y_offset = previous_y - y_pos;

    previous_x = x_pos;
    previous_y = y_pos;

    camera.process_mouse_movement(x_offset, y_offset);
}

// GLFW mouse scroll callback
void scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    Movement::camera.process_mouse_scroll(y_offset);
}

// movement function
void Movement::do_movement()
{
    using Keys::pressed;

    if(pressed[GLFW_KEY_W])
        camera.process_keyboard(Direction::forward, delta_time);
    if(pressed[GLFW_KEY_S])
        camera.process_keyboard(Direction::backward, delta_time);
    if(pressed[GLFW_KEY_A])
        camera.process_keyboard(Direction::left, delta_time);
    if(pressed[GLFW_KEY_D])
        camera.process_keyboard(Direction::right, delta_time);
}
