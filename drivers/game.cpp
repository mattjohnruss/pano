#include <window.h>
#include <camera.h>
#include <buffer.h>
#include <index_buffer.h>
#include <vertex_array.h>
#include <shader.h>
#include <level.h>
#include <texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

class GameWindow : public Window
{
public:
    GameWindow(const unsigned width, const unsigned height, const std::string &level_path = "data/levels/level0.lvl")
        : Window(width, height, "Game Window"),
          camera_({ 0.5f, 0.5f, 1.5f }, { 0.0f, 1.0f, 0.0f }),
          shader_("data/shaders/simple.vert", "data/shaders/simple.frag"),
          level_(level_path),
          previous_time(),
          frame_count(0)
    {
        // define the viewport dimensions
        glViewport(0, 0, width_, height_);

        // enable depth testing
        glEnable(GL_DEPTH_TEST);

        // enable face culling
        glEnable(GL_CULL_FACE);
    }

    ~GameWindow()
    {
    }

    void key_callback(int key, int scancode, int action, int mode) override
    {
        if(action == GLFW_PRESS)
        {
            if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
            {
                glfwSetWindowShouldClose(window_, GL_TRUE);
            }
        }
    }

    void render() override
    {
        double current_time = glfwGetTime();
        double delta_time = current_time - previous_time;
        ++frame_count;

        if(delta_time >= 1.0)
        {
            std::cout << delta_time/frame_count << " (" << frame_count << "fps)\n";
            previous_time = current_time;
            frame_count = 0;
        }

        // clear the background
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_.use();

        glm::mat4 view_mat = glm::mat4(1.0f);
        camera_.view_mat(view_mat);

        glm::mat4 proj_mat = glm::mat4(1.0f);
        proj_mat = glm::perspective(glm::radians(camera_.fov()),
                                    static_cast<float>(width_)/static_cast<float>(height_),
                                    0.01f,
                                    100.0f);

        shader_.set_mat4("u_view_mat", view_mat);
        shader_.set_mat4("u_proj_mat", proj_mat);

        double mouse_x;
        double mouse_y;

        glfwGetCursorPos(window_, &mouse_x, &mouse_y);

        glm::vec2 mouse_pos = glm::vec2((float)mouse_x, (float)mouse_y);

        mouse_pos.x = 2.0*(mouse_pos.x/(width_ - 1.0)) - 1.0;
        mouse_pos.y = 2.0*(1.0 - mouse_pos.y/(height_ - 1.0)) - 1.0;

        // std::printf("%f, %f\n", mouse_pos.x, mouse_pos.y);

        glm::mat4 model_mat = glm::mat4(1.0f);

        //model_mat = glm::translate(model_mat, glm::vec3(0.0f, 0.5f, 0.25f));
        //model_mat = glm::rotate(model_mat, (float)glfwGetTime()/2, glm::vec3(1.0f, 0.0f, 0.0f));
        //model_mat = glm::translate(model_mat, glm::vec3(0.0f, -0.5f, -0.25f));

        shader_.set_mat4("u_model_mat", model_mat);
        shader_.set_vec2("u_light_pos", mouse_pos);

        level_.draw(shader_);

        glfwSwapBuffers(window_);
        glfwWaitEventsTimeout(static_cast<float>(1.0/60.0));
        //glfwWaitEvents();
        //glfwPollEvents();
    }

private:
    Camera camera_;
    Shader shader_;

    Level level_;

    //Buffer vbo_;
    //IndexBuffer ibo_;
    //VertexArray vao_;

    double previous_time;
    unsigned frame_count;
};

int main(int argc, char **argv)
{
    glfwInit();

    //GameWindow window(1200, 900, argv[1]);
    GameWindow window(1920, 1080, argv[1]);
    window.run();

    glfwTerminate();

    return 0;
}
