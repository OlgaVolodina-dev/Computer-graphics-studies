R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 modelTransform;

layout(std140, binding = 2) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    vec4 lightSource1;
    vec4 attenuation1;
    vec4 lightColor1;
    vec4 lightSource2;
    vec4 attenuation2;
    vec4 lightColor2;
    mat4 lightSpaceMatrix[3];
    vec4 lightDir;
};

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragLightPos1;
out vec4 FragLightPos2;
out vec4 FragLightPos3;

void main()
{
    
    gl_Position = projection * view * modelTransform * vec4(aPos, 1.0);
    FragPos = vec3(modelTransform * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    FragLightPos1 = lightSpaceMatrix[0] * vec4(FragPos, 1.0);
    FragLightPos2 = lightSpaceMatrix[1] * vec4(FragPos, 1.0);
    FragLightPos3 = lightSpaceMatrix[2] * vec4(FragPos, 1.0);
    Normal = mat3(transpose(inverse(modelTransform))) * aNormal;
}
)"