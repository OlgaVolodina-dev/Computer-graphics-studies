R"(#version 440 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
};

layout(location = 0) uniform vec3 u_ambient;
layout(location = 1) uniform vec3 u_diffuse;
layout(location = 2) uniform vec3 u_specular;
layout(location = 3) uniform float u_spec_power;

void main()
{

    vec3 lightDir1 = normalize(vec3(0.0, -5.0, -2.0));

    vec3 norm = normalize(Normal);
    float diff = max(abs(dot(norm, lightDir1)), 0.0);
    vec3 diffuse = diff * u_diffuse;

    vec3 viewDir = normalize(vec3(cameraPos) - FragPos);
    vec3 reflectDir = reflect(-lightDir1, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2.0);
    vec3 specular = u_specular * spec;  
    
    FragColor = vec4(u_ambient + diffuse + specular, 1.0);
}
)"