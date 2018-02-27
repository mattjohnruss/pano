#version 450 core

uniform vec2 u_light_pos;
uniform sampler2D u_texture[10];

in vec4 o_colour;
in vec2 o_tex_coords;
in vec4 o_frag_pos;
in float o_tex_id;

out vec4 colour;

void main()
{
    float dist = distance(u_light_pos, o_frag_pos.xy);
    float intensity = 0.1/sqrt(dist);
    int tex_id = int(o_tex_id+0.5);

    colour = o_colour*intensity;
    //colour = intensity*texture(u_texture[tex_id], o_tex_coords);
}
