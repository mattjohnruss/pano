#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct Light
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

in vec3 normal_interp;
in vec3 frag_position;
in vec2 tex_coords_interp;

out vec4 colour;

uniform vec3 view_position;
uniform Material material;
uniform Light light;

void main()
{
    // calculate the direction from the light to the current fragment
    vec3 light_direction_norm = normalize(light.position - frag_position);

    // calculate the distant from the light to the current fragment
    float distance = length(light.position - frag_position);

    // ambient
    // -------
    vec3 ambient = light.ambient*texture(material.diffuse, tex_coords_interp).rgb;
    //vec3 ambient = vec3(0.0f);

    // diffuse
    // -------
    vec3 normal_interp_norm = normalize(normal_interp);
    //vec3 light_direction_norm = normalize(-light.direction);
    float diffuse_amount = max(dot(normal_interp_norm,light_direction_norm), 0.0f);
    vec3 diffuse = diffuse_amount*light.diffuse*texture(material.diffuse, tex_coords_interp).rgb;
    //vec3 diffuse = vec3(0.0f);

    // specular
    // --------
    vec3 view_direction_norm = normalize(view_position - frag_position);
    vec3 reflect_direction_norm = reflect(-light_direction_norm, normal_interp_norm);
    float specular_amount = pow(max(dot(view_direction_norm, reflect_direction_norm), 0.0f), material.shininess);
    vec3 specular = specular_amount*light.specular*texture(material.specular, tex_coords_interp).rgb;
    //vec3 specular = vec3(0.0f);

    // spotlight
    // ---------
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

    // apply the attenuation function to each contribution
    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // final colour
    // ------------
    colour = vec4(ambient + diffuse + specular, 1.0f);
}
