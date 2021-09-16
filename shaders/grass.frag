R"(#version 440 core
out vec4 FragColor;
in vec2 TexCoord;
layout(binding=0) uniform sampler2D grass;

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
    vec4 color = texture(grass, TexCoord);
    if (color.w == 0.0) {
        discard;
    }
    FragColor = color;
}
)"