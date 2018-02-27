#include <renderable2d.h>

Renderable2D::Renderable2D(glm::vec3 position,
                           glm::vec2 size,
                           glm::vec4 colour,
                           Texture2D *texture)
    : position(position), size(size), colour(colour), texture(texture)
{
}

//const glm::vec3 &Renderable2D::position() const
//{
    //return position_;
//}

//const glm::vec2 &Renderable2D::size() const
//{
    //return size_;
//}

//const glm::vec4 &Renderable2D::colour() const
//{
    //return colour_;
//}

//const Texture2D *Renderable2D::texture() const
//{
    //return texture_;
//}

