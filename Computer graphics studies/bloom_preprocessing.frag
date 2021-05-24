R"(#version 420 core
in vec2 texCoord;
out vec4 FragColor;
layout(binding = 0) uniform sampler2D colorTex;

void main() {
	vec4 color = textureLod(colorTex, texCoord, 0.0);
	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0) {
		FragColor = vec4(color.rgb, 1.0);
	} else {
		FragColor = vec4(0.0);
	}
}



)"