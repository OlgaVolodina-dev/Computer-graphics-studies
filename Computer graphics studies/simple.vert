R"(#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 modelTransform;
layout(std140, binding = 2) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    vec4 lightSource1;
    vec4 attenuation1;
    vec4 lightSource2;
    vec4 attenuation2;
    mat4 lightSpaceMatrix;
    vec4 lightDir;
};

void main()
{   
    gl_Position = projection * view *  modelTransform * vec4(aPos, 1.0);
}
)"