#include <shader.h>
#include <camera.h>
#include <graph.h>
#include <texture.h>
#include <model.h>
#include <window.h>

// tell glfw to not include gl headers so we can use glad
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

// for matX operator<<
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>
#include <string>

// vertices and normals of the cube
GLfloat vertices[] =
{
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

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
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
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
std::vector<glm::vec3> point_light_positions
{
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

class PanoWindow : public Window
{
public:
    PanoWindow(const unsigned width, const unsigned height, const std::string &name, const std::string &model_path);

    ~PanoWindow()
    {
        // delete arrays/buffers
        glDeleteVertexArrays(1, &cube_VAO_);
        glDeleteVertexArrays(1, &lamp_VAO_);
        glDeleteBuffers(1, &VBO_);
    }

    void key_callback(int key, int scancode, int action, int mode)
    {
        if(action == GLFW_PRESS)
        {
            if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
                glfwSetWindowShouldClose(window_, GL_TRUE);
            //else if(key == GLFW_KEY_EQUAL)
            //mouse_sensitivity += 0.01f;
            //else if(key == GLFW_KEY_MINUS)
            //mouse_sensitivity -= 0.01f;

            if(key >= 0 && key < 1024)
                pressed_keys_[key] = true;
        }
        else if(action == GLFW_RELEASE && key >= 0 && key < 1024)
            pressed_keys_[key] = false;
    }

    void cursor_position_callback(double x_pos, double y_pos)
    {
        using glm::radians;

        if(first_mouse_)
        {
            previous_x_ = x_pos;
            previous_y_ = y_pos;
            first_mouse_ = false;
        }

        double x_offset = x_pos - previous_x_;
        double y_offset = previous_y_ - y_pos;

        previous_x_ = x_pos;
        previous_y_ = y_pos;

        camera_.process_mouse_movement(x_offset, y_offset);
    }

    void scroll_callback(double x_offset, double y_offset)
    {
        camera_.process_mouse_scroll(y_offset);
    }

    void render();

private:
    // camera
    Camera camera_;

    // keys
    std::vector<bool> pressed_keys_;

    // mouse
    double previous_x_;
    double previous_y_;

    bool first_mouse_;

    // timing
    double delta_time_;
    double current_frame_;
    double previous_frame_;

    // Cube VAO handle
    GLuint cube_VAO_;

    // VBO handle
    GLuint VBO_;

    // lamp VAO handle
    GLuint lamp_VAO_;

    // shaders
    Shader lamp_shader_;
    Shader model_shader_;

    // model
    //Model nanosuit_;
    Model test_model_;

    // projection matrix
    glm::mat4 proj_mat_;

    // view matrix
    glm::mat4 view_mat_;

    // process pressed_keys_ and send movement command to camera
    void do_movement()
    {
        if(pressed_keys_[GLFW_KEY_W])
            camera_.process_keyboard(Direction::forward, delta_time_);
        if(pressed_keys_[GLFW_KEY_S])
            camera_.process_keyboard(Direction::backward, delta_time_);
        if(pressed_keys_[GLFW_KEY_A])
            camera_.process_keyboard(Direction::left, delta_time_);
        if(pressed_keys_[GLFW_KEY_D])
            camera_.process_keyboard(Direction::right, delta_time_);
    }
};

PanoWindow::PanoWindow(const unsigned width, const unsigned height, const std::string &name, const std::string &model_path) :
    Window(width, height, name),
    camera_(glm::vec3(0.0f, 0.0f, 3.0f)),
    pressed_keys_(std::vector<bool>(1024,false)),
    previous_x_(0.0f),
    previous_y_(0.0f),
    first_mouse_(true),
    delta_time_(0.0f),
    current_frame_(0.0f),
    previous_frame_(0.0f),
    lamp_shader_("data/shaders/lamp.vert", "data/shaders/lamp.frag"),
    model_shader_("data/shaders/model_loading.vert", "data/shaders/model_loading.frag"),
    //nanosuit_("data/models/nanosuit/nanosuit.obj"),
    test_model_(model_path.c_str())
{
    // define the viewport dimensions
    glViewport(0, 0, width_, height_);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // enable face culling
    glEnable(GL_CULL_FACE);

    // Cube - generate 1 VAO and assign ID to handle
    glGenVertexArrays(1, &cube_VAO_);

    // generate one VBO buffer
    glGenBuffers(1, &VBO_);

    // bind the VAO
    glBindVertexArray(cube_VAO_);

    // bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

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

    // lamp - generate 1 VAO and assign ID to handle
    glGenVertexArrays(1, &lamp_VAO_);

    // bind the VAO
    glBindVertexArray(lamp_VAO_);

    // allocate and fill data in the bound buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set the vertex attributes pointers for positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
}

void PanoWindow::render()
{
    using glm::radians;

    // update time of current frame
    current_frame_  = glfwGetTime();
    delta_time_     = current_frame_ - previous_frame_;
    previous_frame_ = current_frame_;

    // perform movement
    do_movement();

    // clear the background
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calculate the projection matrix
    proj_mat_ = glm::perspective(
            radians(camera_.fov()),
            static_cast<float>(width_)/static_cast<float>(height_),
            0.1f,
            100.0f);

    // get the view matrix from the camera
    camera_.view_mat(view_mat_);

    // use the light shader and set uniforms
    model_shader_.use();

    // idenitiy model matrix
    glm::mat4 model_mat(1.0f);

    // translate and scale the model a bit
    model_mat = glm::translate(model_mat, glm::vec3(0.0f, -1.75f, 0.0f));
    model_mat = glm::scale(model_mat, glm::vec3(0.1f));

    // set the model matrix in the model shader
    model_shader_.set_mat4("model_mat", model_mat);

    // set the view position
    model_shader_.set_vec3("view_position", camera_.position());

    // directional light
    // -----------------
    model_shader_.set_vec3("directional_light.direction", -0.2f, -1.0f, -0.3f);
    model_shader_.set_vec3("directional_light.ambient", 0.05f);
    model_shader_.set_vec3("directional_light.diffuse", 0.2f);
    model_shader_.set_vec3("directional_light.specular", 0.25f);

    // point lights
    // ------------
    unsigned n_point_light = point_light_positions.size();

    for(unsigned i = 0; i < n_point_light; ++i)
    {
        std::string i_str = std::to_string(i);

        model_shader_.set_vec3(("point_light[" + i_str + "].position").c_str(), point_light_positions[i]);
        model_shader_.set_vec3(("point_light[" + i_str + "].ambient").c_str(), 0.05f);
        model_shader_.set_vec3(("point_light[" + i_str + "].diffuse").c_str(), 0.8f);
        model_shader_.set_vec3(("point_light[" + i_str + "].specular").c_str(), 1.0f);
        model_shader_.set_float(("point_light[" + i_str + "].k_c").c_str(), 1.0f);
        model_shader_.set_float(("point_light[" + i_str + "].k_l").c_str(), 0.09f);
        model_shader_.set_float(("point_light[" + i_str + "].k_q").c_str(), 0.032f);
    }

    // spotlight
    // --------------
    model_shader_.set_vec3("spotlight.position", camera_.position());
    model_shader_.set_vec3("spotlight.direction", camera_.front());
    model_shader_.set_float("spotlight.cos_cutoff", glm::cos(radians(15.0f)));
    model_shader_.set_float("spotlight.cos_outer_cutoff", glm::cos(radians(15.5f)));
    model_shader_.set_vec3("spotlight.ambient", 0.0f);
    model_shader_.set_vec3("spotlight.diffuse", 1.0f);
    model_shader_.set_vec3("spotlight.specular", 1.0f);
    model_shader_.set_float("spotlight.k_c", 1.0f);
    model_shader_.set_float("spotlight.k_l", 0.09f);
    model_shader_.set_float("spotlight.k_q", 0.032f);

    // transformation matrices
    // ---------_--------------
    //model_shader.set_mat4("proj_mat", proj_mat);
    //model_shader.set_mat4("view_mat", view_mat);
    model_shader_.set_mat4("mvp_mat", proj_mat_*view_mat_*model_mat);

    glm::mat3 normal_mat(glm::transpose(glm::inverse(model_mat)));
    model_shader_.set_mat3("normal_mat", normal_mat);

    // material properties
    // -------------------
    model_shader_.set_float("shininess", 32.0f);

    //nanosuit_.draw(model_shader_);
    test_model_.draw(model_shader_);

    // draw point lights as lamps
    // --------------------------

    // use the lamp shader
    lamp_shader_.use();

    // set the projection and view matrix uniforms
    lamp_shader_.set_mat4("proj_mat", proj_mat_);
    lamp_shader_.set_mat4("view_mat", view_mat_);

    // loop over the point lights
    for(unsigned i = 0; i < n_point_light; ++i)
    {
        // set the model matrix using the lamp's position and scale
        model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, point_light_positions[i]);
        model_mat = glm::scale(model_mat, glm::vec3(0.1f));

        // set the model matrix uniform
        lamp_shader_.set_mat4("model_mat", model_mat);

        // render lamp
        glBindVertexArray(lamp_VAO_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // unbind VAO
    glBindVertexArray(0);

    // swap buffers
    glfwSwapBuffers(window_);

    // poll for events
    //glfwPollEvents();
    //glfwWaitEvents();
    glfwWaitEventsTimeout(static_cast<float>(1.0/60.0));
}

int main(int argc, char **argv)
{
    //glfwSwapInterval(1);

    // test: graph object with some nodes
    //Pano::Graph g;
    //g.add_node();
    //g.add_node();
    //g.add_node();

    // init glfw
    glfwInit();

    // create a window object
    PanoWindow window(1600, 900, "Pano", argv[1]);

    // start the game loop
    window.run();

    // terminate glfw
    glfwTerminate();

    return 0;
}
