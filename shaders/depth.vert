R"(#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 modelTransform;
layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 projview;
};

void main()
{   
    gl_Position = projview *  modelTransform * vec4(aPos, 1.0);
}
)"