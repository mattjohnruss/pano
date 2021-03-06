#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

enum class Direction
{
    forward,
    backward,
    left,
    right
};

class Camera
{
public:
    Camera(const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f),
           const glm::vec3 &up  = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw   = -90.0f,
           GLfloat pitch = 0.0f);

    ~Camera()
    {
    }

    void view_mat(glm::mat4 &view_mat) const;

    void process_keyboard(const Direction dir, const GLfloat delta_time);
    void process_mouse_movement(const GLfloat x_offset,
                                const GLfloat y_offset,
                                const GLboolean const_pitch = true);
    void process_mouse_scroll(const GLfloat y_offset);

    const GLfloat &fov() const;
    const glm::vec3 &position() const;
    const glm::vec3 &front() const;

private:
    // update the camera vectors
    void update_vectors();

    // vectors
    glm::vec3 pos_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    // Euler angles
    GLfloat yaw_;
    GLfloat pitch_;

    // params
    GLfloat speed_;
    GLfloat mouse_sensitivity_;

    // field of view
    GLfloat fov_;
};
