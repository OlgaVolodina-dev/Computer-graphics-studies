R"(#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 ourColor;


void main()
{
    gl_Position = vec4(aPos, 0.5, 1.0);
}
)"