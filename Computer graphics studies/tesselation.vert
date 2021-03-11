R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texPos;
layout (location = 3) in mat4 modelTransform;

out mat4 TCModelTransform;
out vec2 TCTexPos;

void main()
{
    TCTexPos = texPos;
    TCModelTransform = modelTransform;
    gl_Position =  vec4(aPos, 1.0);
}
)"