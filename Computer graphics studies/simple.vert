R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 modelTransform;



layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
    float pad;
    vec3 lightSource;
};


void main()
{
    
    gl_Position = projection * view * modelTransform * vec4(aPos, 1.0);
}
)"