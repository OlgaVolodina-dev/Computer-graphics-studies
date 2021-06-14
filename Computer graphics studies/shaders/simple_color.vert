R"(#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 3) in mat4 modelTransform;
layout (location = 0) uniform mat4 projView;
layout (location = 1) uniform vec4 clipPlane;

out vec2 texCoords;

void main()
{   
    vec4 wordPosition = modelTransform * vec4(aPos, 1.0);
    gl_ClipDistance[0] = dot(wordPosition, clipPlane);
    texCoords = aTexCoord;
    gl_Position = projView * wordPosition;

}
)"