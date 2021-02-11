R"(#version 330 core
layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec3 lightSource;
};

out vec4 FragColor;
  
uniform sampler2D texture1;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

void main()
{
    vec3 lightColor = vec3(texture(texture1, TexCoord));

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightSource - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4(diffuse + ambient, 1.0);
}
)"