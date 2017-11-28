#include <camera.h>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &pos,
               const glm::vec3 &up,
               const GLfloat yaw,
               const GLfloat pitch)
    : pos_(pos),
      world_up_(up),
      yaw_(yaw),
      pitch_(pitch),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      speed_(1.0f),
      mouse_sensitivity_(0.25f),
      fov_(45.0f)
{
    update_vectors();
}

void Camera::update_vectors()
{
    front_ =
        glm::normalize(
            glm::vec3(
                std::cos(glm::radians(yaw_))*std::cos(glm::radians(pitch_)),
                std::sin(glm::radians(pitch_)),
                std::sin(glm::radians(yaw_))*std::cos(glm::radians(pitch_))));
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_));
}

void Camera::view_mat(glm::mat4 &view_mat) const
{
    view_mat = glm::lookAt(pos_, pos_ + front_, up_);
}

void Camera::process_keyboard(const Direction dir, const GLfloat delta_time)
{
    GLfloat velocity = speed_*delta_time;
    switch(dir)
    {
        case Direction::forward:
            pos_ += front_*speed_*delta_time;
            break;
        case Direction::backward:
            pos_ -= front_*speed_*delta_time;
            break;
        case Direction::left:
            pos_ -= right_*speed_*delta_time;
            break;
        case Direction::right:
            pos_ += right_*speed_*delta_time;
            break;
    }
}

void Camera::process_mouse_movement(
        const GLfloat x_offset, const GLfloat y_offset, const GLboolean const_pitch)
{
    yaw_   += x_offset*mouse_sensitivity_;
    pitch_ += y_offset*mouse_sensitivity_;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if(const_pitch)
    {
        if(pitch_ > 89.0f)
            pitch_ = 89.0f;
        if(pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    update_vectors();
}

void Camera::process_mouse_scroll(const GLfloat y_offset)
{
    if(fov_ >= 1.0f && fov_ <= 45.0f)
        fov_ -= y_offset;
    if(fov_ <= 1.0f)
        fov_ = 1.0f;
    if(fov_ >= 45.0f)
        fov_ = 45.0f;
}

const GLfloat &Camera::fov() const
{
    return fov_;
}

const glm::vec3 &Camera::position() const
{
    return pos_;
}

const glm::vec3 &Camera::front() const
{
    return front_;
}
