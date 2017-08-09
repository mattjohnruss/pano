#version 330 core

in vec2 tex_coord_interp;

out vec4 colour;

uniform sampler2D texture1_unif;
uniform sampler2D texture2_unif;

void main()
{
    colour = mix(texture(texture1_unif,tex_coord_interp),
                 texture(texture2_unif,tex_coord_interp),
                 0.2f);
}
