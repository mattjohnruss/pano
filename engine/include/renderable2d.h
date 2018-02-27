#pragma once

#include <texture.h>

#include <glm/glm.hpp>

class Renderable2D
{
public:
    ////virtual void draw() const = 0;

    //const glm::vec3 &position() const;
    //const glm::vec2 &size() const;
    //const glm::vec4 &colour() const;
    //const Texture2D *texture() const;
    
    Renderable2D(glm::vec3 position,
                 glm::vec2 size,
                 glm::vec4 colour,
                 Texture2D *texture);

//protected:
    glm::vec3 position;
    glm::vec2 size;
    glm::vec4 colour;
    Texture2D *texture;
};
