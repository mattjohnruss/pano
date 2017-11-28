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
    //float dist = distance(vec3(u_light_pos.xy, -1.0), o_frag_pos.xyz);
    float dist = distance(u_light_pos, o_frag_pos.xy);
    float intensity = 0.1/sqrt(dist);
    //colour = o_colour*intensity;
    int tex_id = int(o_tex_id);

    //colour.rg = o_tex_coords.xy;
    //colour.b = 0.0;
    //colour.a = 1.0;
    colour = intensity*texture(u_texture[tex_id], o_tex_coords);

    //colour = vec4(vec3(o_tex_id), 0.0)/9.0;
    //colour = vec4(u_light_pos.zzz, 1.0);
    //colour = vec4(u_light_pos.x, u_light_pos.y, u_light_pos.z, 1.0);
    //colour = vec4(o_frag_pos.x, o_frag_pos.y, 1.0, 1.0);
    //colour = vec4(gl_FragCoord.x/1920.0, gl_FragCoord.y/1080.0, 1.0, 1.0);
}
