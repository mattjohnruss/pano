#pragma once

// tell glfw to not include gl headers so we can use glad
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
    // constructor
    Window(const unsigned width,
           const unsigned height,
           const std::string &name,
           const int minor_gl_version = 4,
           const int major_gl_version = 5);

    // destructor
    virtual ~Window();

    // return the window width and height
    const unsigned &width() const;
    const unsigned &height() const;

    // make the window current context
    void make_current() const;

    // actual input callbacks that will be overridden in a derived class
    virtual void key_callback(int key, int scancode, int action, int mode);
    virtual void cursor_position_callback(double x_pos, double y_pos);
    virtual void scroll_callback(double x_offset, double y_offset);

    // render function
    virtual void render();

    // the game loop
    void run();

protected:
    // pointer underlying GLFWwindow
    GLFWwindow *window_;

    // height and width of the window
    unsigned width_;
    unsigned height_;

    // name of the window
    std::string name_;

private:
    // static dispatch methods for the input callbacks
    static void sd_key_callback(GLFWwindow *glfw_window, int key, int scancode, int action, int mode);
    static void sd_cursor_position_callback(GLFWwindow *glfw_window, double x_pos, double y_pos);
    static void sd_scroll_callback(GLFWwindow *glfw_window, double x_offset, double y_offset);
};
