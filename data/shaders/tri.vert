#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 tex_coord;

out vec3 colour_interp;
out vec2 tex_coord_interp;

uniform mat4 model_mat_unif;
uniform mat4 view_mat_unif;
uniform mat4 proj_mat_unif;

void main()
{
    gl_Position = proj_mat_unif*view_mat_unif*model_mat_unif*vec4(position, 1.0f);
    colour_interp = colour;
    tex_coord_interp = tex_coord;
}
