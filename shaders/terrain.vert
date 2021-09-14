R"(#version 440 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 modelTransform;

layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
};

layout(binding=1) uniform sampler2D heightmapTex;

out vec3 Normal;
out vec3 Position;
out vec2 TexCoord;
void main()
{   
    highp float height = texture(heightmapTex, aTexCoord).r;
    vec3 position = vec3(aPos.x, height, aPos.y);
    gl_Position = projection * view *  modelTransform * vec4(position, 1.0);
    Normal =  vec3(0.0, 1.0, 0.0);
    Position = position;
    TexCoord = aTexCoord;

}
)"