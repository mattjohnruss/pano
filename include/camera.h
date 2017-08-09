#ifndef CAMERA_H_
#define CAMERA_H_

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
private:
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

    // update the camera vectors
    void update_vectors();

public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up  = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw   = -90.0f,
           GLfloat pitch = 3.0f);

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
};

#endif // CAMERA_H_
