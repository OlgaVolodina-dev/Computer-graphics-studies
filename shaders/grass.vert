R"(#version 440 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 worldPosition;
out vec2 TexCoord;
layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    vec4 cameraRight;
    vec4 cameraUp;
};

void main()
{
    TexCoord = aTexCoord;
    vec3 cameraSpacePosition = worldPosition - cameraUp.xyz * aPos.y - cameraRight.xyz * aPos.x;
    gl_Position = projection * view  * vec4(cameraSpacePosition, 1.0);
}
)"