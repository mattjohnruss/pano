#version 330 core

struct PointLight
{
    // position of the light
    vec3 position;

    // ambient, diffuse and specular light colours
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // coefficients for the quadratic attenuation function
    float k_c;
    float k_l;
    float k_q;
};

#define N_POINT_LIGHTS 4u

in vec3 normal_interp;
in vec3 frag_position;

uniform vec3 view_position;
uniform PointLight point_light[N_POINT_LIGHTS];
uniform float shininess;
uniform float pert[3];

uniform sampler3D texture_hatching;

out vec4 colour;

// calculate point light contribution
// assumes vector parameters are normalised (where appropriate)
float calc_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    // calculate the direction from the current fragment to the light
    vec3 light_direction_norm = normalize(light.position - frag_position);

    // calculate the halfway vector
    vec3 halfway_direction_norm = normalize(light_direction_norm + view_direction);

    // calculate the distance from the current fragment to the light
    float distance = length(light.position - frag_position);

    // ambient
    float ambient_amount = 0.025;
    //float ambient_amount = 0.0;

    // diffuse
    float diffuse_amount = max(dot(normal, light_direction_norm), 0.0);
    //float diffuse_amount = 0.0;

    // specular
    //float specular_amount = pow(max(dot(halfway_direction_norm, normal), 0.0), 4.0*shininess);
    //float specular_amount = 0.0;
    vec3 reflected = reflect(-light_direction_norm, normal);
    float specular_amount = pow(max(dot(view_direction, reflected), 0.0), shininess);

    // attenuation
    // -----------
    // calculate the attenuation function for the location of the current fragment
    float attenuation = 1.0/(light.k_c + light.k_l*distance + light.k_q*distance*distance);

    // return total
    return attenuation*(ambient_amount + diffuse_amount + specular_amount);
}

// "random" float 0.0--1.0 for a pixel with coords co
float rand(vec2 co)
{
  return fract(sin(dot(co.xy, vec2(12.9898, 78.233)))*43758.5453);
}

void main()
{
    // normalise the interpolated normal and view direction vectors
    vec3 normal_interp_norm = normalize(normal_interp);
    vec3 view_direction_norm = normalize(view_position - frag_position);

    // total intensity
    float intensity = 0.0;

    // add contributions from point lights
    for(uint i = 0u; i < N_POINT_LIGHTS; ++i)
    {
        intensity += calc_point_light(point_light[i], normal_interp_norm, frag_position, view_direction_norm);
    }

    intensity = clamp(intensity, 0.0, 1.0);

    ivec3 tex_hatching_size = textureSize(texture_hatching, 0);

    vec3 tex_coords = vec3(0.0);

    tex_coords.xy = gl_FragCoord.xy/tex_hatching_size.x;
    tex_coords.z = intensity;

    //tex_coords.x += 0.01*rand(gl_FragCoord.xy + pert[0]);
    //tex_coords.y += 0.01*rand(gl_FragCoord.xy + pert[1]);
    //tex_coords.z += 0.1*rand(gl_FragCoord.xy + pert[2]);

    vec3 hatching = texture(texture_hatching, tex_coords).rgb;

    // final colour
    // ------------
    colour = vec4(hatching, 1.0);

    //colour = vec4(tex_coords.x, tex_coords.y, tex_coords.z, 1.0);

    // version without using texture
    //colour = vec4(1.0, 1.0, 1.0, 1.0);

    //float x_p_y_pert = gl_FragCoord.x + rand(gl_FragCoord.xy + pert[0]) +
                       //gl_FragCoord.y + rand(gl_FragCoord.xy + pert[1]);

    //float x_m_y_pert = gl_FragCoord.x + rand(gl_FragCoord.xy + pert[0]) -
                       //gl_FragCoord.y + rand(gl_FragCoord.xy + pert[1]);

    //const float period = 20.0;
    //const float threshold = 1.5;

    //const float off = period/2.0;

    //if(intensity < 0.85)
    //{
        //// hatch from left top corner to right bottom
        //if(abs(mod(x_p_y_pert, period)) < threshold)
        //{
            //colour = vec4(0.0, 0.0, 0.0, 1.0);
        //}
    //}
    //if(intensity < 0.75)
    //{
        //// hatch from right top corner to left boottom
        //if(mod(x_m_y_pert, period) < threshold)
        //{
            //colour = vec4(0.0, 0.0, 0.0, 1.0);
        //}
    //}
    //if(intensity < 0.5)
    //{
        //// hatch from left top to right bottom
        //if(mod(x_p_y_pert - off, period) < threshold)
        //{
            //colour = vec4(0.0, 0.0, 0.0, 1.0);
        //}
    //}
    //if(intensity < 0.25)
    //{
        //// hatch from right top corner to left bottom
        //if(mod(x_m_y_pert - off, period) < threshold)
        //{
            //colour = vec4(0.0, 0.0, 0.0, 1.0);
        //}
    //}
}
