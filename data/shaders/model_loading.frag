#version 330 core

struct DirectionalLight
{
    // direction of the light
    vec3 direction;

    // ambient, diffuse and specular light colours
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

struct Spotlight
{
    // position and direction of the light
    vec3 position;
    vec3 direction;

    // cosine of the spotlight cutoff angle
    float cos_cutoff;

    // cosine of the spotlight outer cutoff angle
    float cos_outer_cutoff;

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
uniform DirectionalLight directional_light;
uniform PointLight point_light[N_POINT_LIGHTS];
uniform Spotlight spotlight;
//uniform uint n_point_light;
uniform float shininess;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

// calculate directional light contribution
// assumes vector parameters are normalised (where appropriate)
vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction)
{
    // calculate the direction of the light - doesn't depend on position for
    // directional light
    vec3 light_direction_norm = normalize(-light.direction);

    // ambient
    vec3 ambient = light.ambient*texture(texture_diffuse1, tex_coords_interp).rgb;

    // diffuse
    float diffuse_amount = max(dot(normal, light_direction_norm), 0.0f);
    vec3 diffuse = diffuse_amount*light.diffuse*texture(texture_diffuse1, tex_coords_interp).rgb;

    // specular
    vec3 reflect_direction_norm = reflect(-light_direction_norm, normal);
    float specular_amount = pow(max(dot(view_direction, reflect_direction_norm), 0.0f), shininess);
    vec3 specular = specular_amount*light.specular*texture(texture_specular1, tex_coords_interp).rgb;

    // return total
    return ambient + diffuse + specular;
}

// calculate point light contribution
// assumes vector parameters are normalised (where appropriate)
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    // calculate the direction from the current fragment to the light
    vec3 light_direction_norm = normalize(light.position - frag_position);

    // calculate the distance from the current fragment to the light
    float distance = length(light.position - frag_position);

    // ambient
    vec3 ambient = light.ambient*texture(texture_diffuse1, tex_coords_interp).rgb;

    // diffuse
    float diffuse_amount = max(dot(normal, light_direction_norm), 0.0f);
    vec3 diffuse = diffuse_amount*light.diffuse*texture(texture_diffuse1, tex_coords_interp).rgb;

    // specular
    vec3 reflect_direction_norm = reflect(-light_direction_norm, normal);
    float specular_amount = pow(max(dot(view_direction, reflect_direction_norm), 0.0f), shininess);
    vec3 specular = specular_amount*light.specular*texture(texture_specular1, tex_coords_interp).rgb;

    // attenuation
    // -----------
    // calculate the attenuation function for the location of the current fragment
    float attenuation = 1.0/(light.k_c + light.k_l*distance + light.k_q*distance*distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // return total
    return ambient + diffuse + specular;
}

// calculate spotlight contribution
// assumes vector parameters are normalised (where appropriate)
vec3 calc_spotlight(Spotlight light, vec3 normal, vec3 frag_position, vec3 view_direction)
{
    // calculate the direction from the current fragment to the light
    vec3 light_direction_norm = normalize(light.position - frag_position);

    // calculate the distance from the current fragment to the light
    float distance = length(light.position - frag_position);

    // ambient
    vec3 ambient = light.ambient*texture(texture_diffuse1, tex_coords_interp).rgb;

    // diffuse
    float diffuse_amount = max(dot(normal, light_direction_norm), 0.0f);
    vec3 diffuse = diffuse_amount*light.diffuse*texture(texture_diffuse1, tex_coords_interp).rgb;

    // specular
    vec3 reflect_direction_norm = reflect(-light_direction_norm, normal);
    float specular_amount = pow(max(dot(view_direction, reflect_direction_norm), 0.0f), shininess);
    vec3 specular = specular_amount*light.specular*texture(texture_specular1, tex_coords_interp).rgb;

    // cutoff
    // ------
    //dynamic outer cutoff calculation based on distance from the fragment to the light
    float outer_cutoff_base = acos(light.cos_outer_cutoff);
    float cos_outer_cutoff = cos(clamp(outer_cutoff_base*(pow(distance+1.0,0.5)),outer_cutoff_base,1.5*outer_cutoff_base));

    // cosine of the angle between the light-frag direction and the light direction
    float cos_theta = dot(light_direction_norm, normalize(-light.direction));
    float epsilon = light.cos_cutoff - cos_outer_cutoff;
    float intensity = clamp((cos_theta - cos_outer_cutoff)/epsilon, 0.0f, 1.0f);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    // -----------
    // calculate the attenuation function for the location of the current fragment
    float attenuation = 1.0/(light.k_c + light.k_l*distance + light.k_q*distance*distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // return total
    return ambient + diffuse + specular;
}

void main()
{
    // normalise the interpolated normal and view direction vectors
    vec3 normal_interp_norm = normalize(normal_interp);
    vec3 view_direction_norm = normalize(view_position - frag_position);

    // total colour
    vec3 total_colour = vec3(0.0f);

    // add contribution from directional light
    total_colour += calc_directional_light(directional_light, normal_interp_norm, view_direction_norm);

    // add contributions from point lights
    //for(uint i = 0u; i < n_point_light; ++i)
    for(uint i = 0u; i < N_POINT_LIGHTS; ++i)
    {
        total_colour += calc_point_light(point_light[i], normal_interp_norm, frag_position, view_direction_norm);
    }

    // add contribution from spotlight
    //total_colour += calc_spotlight(spotlight, normal_interp_norm, frag_position, view_direction_norm);

    // final colour
    // ------------
    colour = vec4(total_colour, 1.0f);
}
