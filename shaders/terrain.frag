R"(#version 440 core
out vec4 FragColor;
in vec3 Normal;
in vec3 Position;
in vec2 TexCoord;


layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
};

layout(binding=0) uniform usampler2D biomeTex;
layout(binding=1) uniform sampler2D sand;
layout(binding=2) uniform sampler2D grass;
layout(binding=3) uniform sampler2D forest;

void main()
{
    uint biome = texture(biomeTex, TexCoord).r;
    vec3 color =  texture(grass, TexCoord * 25.0).xyz;
    if (biome == 0U) {
        color = texture(sand, TexCoord * 25.0).xyz;
    } else if (biome == 1U) {
        color = texture(forest, TexCoord * 25.0).xyz;
    }
    vec3 lightDir1 = normalize(vec3(0.0, -5.0, -2.0));
    float diffuseStrength = 1.0;
    vec3 norm = normalize(Normal);
    float diff = max(abs(dot(norm, lightDir1)), 0.0) ;
    FragColor = vec4(diff * color, 1.0);
    //FragColor = vec4(abs(norm), 1.0);
}
)"