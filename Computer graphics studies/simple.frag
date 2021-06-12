R"(#version 440 core
out vec4 FragColor;

layout(location = 0) uniform vec3 color;
  
void main()
{
    FragColor = vec4(color,  1.0);
}
)"