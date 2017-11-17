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
in vec2 tex_coords_interp;

out vec4 colour;

uniform vec3 view_position;
uniform PointLight point_light[N_POINT_LIGHTS];

//uniform float b;
//uniform float y;
uniform vec3 blue;
uniform vec3 yellow;
uniform float alpha;
uniform float beta;
uniform float shininess;

uniform float expansion;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform bool outlines;

const uint n_level = 4u;

// calculate gooch light contribution
// assumes vector parameters are normalised (where appropriate)
vec3 gooch_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    //vec3 blue = vec3(0.0, 0.0, b);
    //vec3 yellow = vec3(y, y, 0.0);

    //vec3 c_cool_colour =
        //clamp(blue + alpha*texture(texture_diffuse1, tex_coords_interp).rgb, 0.0, 1.0);
    //vec3 c_warm_colour =
        //clamp(yellow + beta*texture(texture_diffuse1, tex_coords_interp).rgb, 0.0, 1.0);

    vec3 c_cool_colour =
        clamp(blue + alpha*light.diffuse, 0.0, 1.0);
    vec3 c_warm_colour =
        clamp(yellow + beta*light.diffuse, 0.0, 1.0);

    // calculate the direction from the current fragment to the light
    vec3 light_direction_norm = normalize(light.position - frag_position);

    // calculate the halfway vector
    //vec3 halfway_direction_norm = normalize(light_direction_norm + view_direction);

    // calculate the distance from the current fragment to the light
    float distance = length(light.position - frag_position);

    // ambient
    //vec3 ambient = light.ambient*texture(texture_diffuse1, tex_coords_interp).rgb;
    vec3 ambient = vec3(0.0);

    // diffuse
    float diffuse_amount = 0.5*(1.0 + dot(normal, light_direction_norm));
    //vec3 diffuse = light.diffuse*mix(c_cool_colour, c_warm_colour, diffuse_amount);
    vec3 diffuse = mix(c_cool_colour, c_warm_colour, diffuse_amount);

    // specular
    //float specular_amount = pow(max(dot(halfway_direction_norm, normal), 0.0f), 4.0*shininess);
    vec3 reflect_direction_norm = reflect(-light_direction_norm, normal);
    float specular_amount = pow(max(dot(view_direction, reflect_direction_norm), 0.0f), shininess);
    //float specular_amount = 0.0f;
    vec3 specular = specular_amount*light.specular*texture(texture_specular1, tex_coords_interp).rgb;
    //vec3 specular = specular_amount*light.specular;

    // attenuation
    // -----------
    // calculate the attenuation function for the location of the current fragment
    float attenuation = 1.0/(light.k_c + light.k_l*distance + light.k_q*distance*distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //diffuse = ceil(n_level*diffuse)/n_level;
    //diffuse = mix(texture(texture_diffuse1, tex_coords_interp).rgb, diffuse, 0.9);

    // return total
    vec3 total = (ambient + diffuse + specular);
    //total = mix(texture(texture_diffuse1, tex_coords_interp).rgb, total, 0.8);
    total = 0.3*ceil(n_level*total)/n_level;
    //total = mix(total,texture(texture_diffuse1, tex_coords_interp).rgb,0.4)/3;
    return total;
}

void main()
{
    //if(expansion > 0.0f)
    //if(!gl_FrontFacing)
    if(outlines)
    {
        colour = vec4(vec3(0.0f), 1.0f);
        return;
    }

    // normalise the view direction vectors
    vec3 view_direction_norm = normalize(view_position - frag_position);

    // total colour
    vec3 total_colour = vec3(0.0f);

    // add contributions from point lights
    for(uint i = 0u; i < N_POINT_LIGHTS; ++i)
    {
        total_colour += gooch_light(point_light[i], normalize(normal_interp), frag_position, view_direction_norm);
    }

    // final colour
    // ------------
    colour = vec4(total_colour, 1.0f);
}
