#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 model_mat;
//uniform mat4 view_mat;
//uniform mat4 proj_mat;
uniform mat4 mvp_mat;
uniform mat3 normal_mat;

out vec3 normal_interp;
out vec3 frag_position;
out vec2 tex_coords_interp;

void main()
{
    frag_position = vec3(model_mat*vec4(position, 1.0f));
    normal_interp = normal_mat*normal;
    tex_coords_interp = tex_coords;

    //gl_Position = proj_mat*view_mat*vec4(frag_position, 1.0f);
    gl_Position = mvp_mat*vec4(position, 1.0f);
}
