R"(#version 440 core
out vec4 FragColor;

in vec2 texCoords;
layout(binding=0) uniform sampler2D colorTex;

void main()
{
    FragColor = texture(colorTex, texCoords);
}
)"