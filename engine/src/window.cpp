#include <window.h>

#include <glad/glad.h>

#include <iostream>

Window::Window(
        const unsigned width,
        const unsigned height,
        const std::string &name,
        const int minor_gl_version,
        const int major_gl_version) :
    width_(width),
    height_(height),
    name_(name)
{
    // request opengl core profile and disable window resizing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, minor_gl_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, major_gl_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // create a GLFWwindow
    window_ =
        glfwCreateWindow(width_, height_, name_.c_str(), glfwGetPrimaryMonitor(), NULL);
        //glfwCreateWindow(width_, height_, name_.c_str(), NULL, NULL);

    // check the window was created successfully and quit if not
    if(window_ == NULL)
    {
        std::cerr << "Window::Window - failed to create GLFW window\n";
        glfwTerminate();
        std::exit(1);
    }

    // make this window the current context
    //make_current();
    glfwMakeContextCurrent(window_);

    // set the "user pointer" of the glfw window to the current object
    glfwSetWindowUserPointer(window_, this);

    // set the callbacks to the static dispatch methods
    glfwSetKeyCallback(window_, sd_key_callback);
    glfwSetCursorPosCallback(window_, sd_cursor_position_callback);
    glfwSetScrollCallback(window_, sd_scroll_callback);

    // disable vsync (for performance analysis)
    glfwSwapInterval(0);

    // enable mouse capturing
    //glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set the opengl function pointers using glad
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cerr << "Window::Window - GLAD failed to initialize OpenGL context\n";
        std::exit(1);
    }
}

Window::~Window()
{
    // destroy the GLFWwindow
    glfwDestroyWindow(window_);
}

const unsigned &Window::width() const
{
    // return the window width
    return width_;
}

const unsigned &Window::height() const
{
    // return the window height
    return height_;
}

void Window::make_current() const
{
    // make this the current window
    glfwMakeContextCurrent(window_);
}

void Window::key_callback(int key, int scancode, int action, int mode)
{
    // do nothing by default
}

void Window::cursor_position_callback(double x_pos, double y_pos)
{
    // do nothing by default
}

void Window::scroll_callback(double x_offset, double y_offset)
{
    // do nothing by default
}

void Window::update_state()
{
    // do nothing by default
}

void Window::render()
{
    // do nothing by default
}

void Window::run()
{
    constexpr unsigned target_ups = 60;
    constexpr double seconds_per_update = 1.0/static_cast<double>(target_ups);

    double previous_time = glfwGetTime();
    double delta = 0.0;

    double timer = glfwGetTime();

    unsigned frames = 0;
    unsigned updates = 0;

    // loop until the window is closed
    while(!glfwWindowShouldClose(window_))
    {
        // check for errors
#ifdef PANO_DEBUG
        GLenum gl_error;
        while((gl_error = glGetError()) != GL_NO_ERROR)
        {
            std::cerr << "Window::run - GL error(" << gl_error << ")\n";
        }
#endif
        // TODO separate input processing into its own function

        double current_time = glfwGetTime();
        delta += (current_time - previous_time)/seconds_per_update;
        previous_time = current_time;

        while(delta >= 1.0)
        {
            // update the game state
            update_state();
            ++updates;
            delta -= 1.0;
        }

        // for now just do everything in the render function
        render();
        ++frames;

        if((glfwGetTime() - timer) > 1.0)
        {
            timer += 1.0;
            std::cout << "updates: " << updates << ", frames: " << frames << '\n';

            updates = 0;
            frames = 0;
        }
    }
}

void Window::sd_key_callback(
        GLFWwindow *glfw_window, int key, int scancode, int action, int mode)
{
    // get the Window object from the glfw user pointer
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

    // call the actual callback
    window->key_callback(key, scancode, action, mode);
}

void Window::sd_cursor_position_callback(
    GLFWwindow *glfw_window, double x_pos, double y_pos)
{
    // get the Window object from the glfw user pointer
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

    // call the actual callback
    window->cursor_position_callback(x_pos, y_pos);
}

void Window::sd_scroll_callback(
    GLFWwindow *glfw_window, double x_offset, double y_offset)
{
    // get the Window object from the glfw user pointer
    Window *window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));

    // call the actual callback
    window->scroll_callback(x_offset, y_offset);
}
