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

layout(binding=0) uniform sampler2D sand;
layout(binding=1) uniform sampler2D grass;

  
void main()
{
    vec3 color =  texture(grass, TexCoord).xyz;
    if (Position.y < -4.0) {
        color = texture(sand, TexCoord).xyz;
    }
    vec3 lightDir1 = normalize(vec3(0.0, -5.0, -2.0));
    float diffuseStrength = 1.0;
    vec3 norm = normalize(Normal);
    float diff = max(abs(dot(norm, lightDir1)), 0.0) ;
    FragColor = vec4(diff * color, 1.0);
}
)"