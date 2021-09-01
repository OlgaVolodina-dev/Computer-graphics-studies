R"(#version 440 core
out vec4 FragColor;

in vec2 texCoord;
layout(binding =0 ) uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, texCoord);
}


)"