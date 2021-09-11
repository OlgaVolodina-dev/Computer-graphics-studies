R"(#version 440 core
out vec4 FragColor;
in vec3 Normal;

layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
};

layout(location = 0) uniform vec3 color;
  
void main()
{

    vec3 lightDir1 = normalize(vec3(0.0, -5.0, -2.0));
    float diffuseStrength = 1.0;
    vec3 norm = normalize(Normal);
    float diff = max(abs(dot(norm, lightDir1)), 0.0);
    FragColor = vec4(diff);
}
)"