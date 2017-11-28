#pragma once

#include <glm/glm.hpp>

// vertex data structure
// stores the position vector, normal vector and texture coords of a vertex
// It's safe to use a struct in a gl buffer as long as we specify the stride
// correctly (e.g. using the offsetof macro) to account for alignment/padding
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec4 colour;
};

// enum for the fields of a vertex so we can pass them as function args
enum class VertexDataType
{
    POSITION = 0,
    NORMAL,
    TEX_COORDS,
    COLOUR
};
