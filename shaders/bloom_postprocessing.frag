R"(#version 440 core
in vec2 texCoord;
out vec4 FragColor;
layout(location = 0) uniform float exposure;
layout(binding = 0) uniform sampler2D hdrBuffer;
layout(binding = 1) uniform sampler2D bloomBuffer;



void main() {

    const float gamma = 1.3;
    vec3 hdrColor = texture(hdrBuffer, texCoord).rgb;
    vec3 bloomColor = textureLod(bloomBuffer, texCoord, 0.0).rgb;
    hdrColor += bloomColor;
  
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
  
}
)"