#version 330 core

in vec3 colour_interp;
in vec2 tex_coord_interp;

out vec4 colour;

uniform float colour_unif;
uniform sampler2D texture_unif;

void main()
{
    colour = colour_unif*texture(texture_unif,tex_coord_interp)*vec4(colour_interp,1.0f);
}
