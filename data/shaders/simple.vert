#version 450 core

layout (location = 0) in vec3  a_position;
layout (location = 1) in vec2  a_tex_coords;
layout (location = 2) in vec4  a_colour;
layout (location = 3) in float a_tex_id;

uniform mat4 u_model_mat;
uniform mat4 u_view_mat;
uniform mat4 u_proj_mat;

out vec4 o_colour;
out vec2 o_tex_coords;
out vec4 o_frag_pos;
out float o_tex_id;

void main()
{
    o_colour = a_colour;
    o_tex_coords = a_tex_coords;
    o_frag_pos = u_view_mat*u_model_mat*vec4(a_position, 1.0);
    o_tex_id = a_tex_id;

    gl_Position = u_proj_mat*u_view_mat*u_model_mat*vec4(a_position, 1.0f);
}
