R"(#version 440 core

layout (location = 0) uniform float moveFactor;

layout(std140, binding = 2) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    vec4 lightSource1;
    vec4 attenuation1;
    vec4 lightColor1;
    vec4 lightSource2;
    vec4 attenuation2;
    vec4 lightColor2;
    mat4 lightSpaceMatrix[3];
    vec4 lightDir;
};

out vec4 FragColor;
  
layout(binding=0) uniform sampler2D texture1;
layout(binding=3) uniform sampler2D reflectionTex;
layout(binding=4) uniform sampler2D refractionTex;
layout(binding=5) uniform sampler2D dudv;
layout(binding=6) uniform sampler2D normalTex;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 FragPosScreen;

#define WAVE_STRENGTH 0.02
void main()
{
    vec3 fragPosScreen = FragPosScreen.xyz / FragPosScreen.w * 0.5 + 0.5;

    vec2 distortionsCoords = texture(dudv, vec2(TexCoord.x + moveFactor, TexCoord.y)).rg * 0.1;
    distortionsCoords = TexCoord + vec2(distortionsCoords.x, distortionsCoords.y + moveFactor);
    vec2 distortions = (texture(dudv, distortionsCoords).rg * 2.0 - 1.0) * WAVE_STRENGTH;

    vec2 reflectTexCoords = clamp((fragPosScreen.xy + distortions), 0.001, 0.999);
    vec2 refractTexCoords = clamp((vec2(fragPosScreen.x, 1.0-fragPosScreen.y) + distortions), 0.001, 0.999);
    vec4 reflectionColor = texture(reflectionTex, reflectTexCoords);
    vec4 refractionColor = texture(refractionTex, refractTexCoords);

    vec3 cameraVector  = normalize(cameraPos.xyz - FragPos);
    vec3 reflectionVector = vec3(0.0, 1.0, 0.0);
    
    vec3 normal = texture(normalTex, distortions).rgb;
    normal = vec3(normal.r * 2.0 - 1.0, normal.b, normal.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 reflectDir = reflect(normalize(-lightDir.xyz), normal);
    float specular = max(dot(reflectDir, cameraVector), 0.0);
    specular = pow(specular, 20.0);
    
    vec3 lightColor = specular * vec3(1.0) * 0.3;
    
    float coef = dot(cameraVector, reflectionVector);
    vec4 outColor = mix(refractionColor, reflectionColor , coef) ;
    FragColor = mix(outColor, vec4(0.0, 0.3, 0.5, 0.1), 0.2) + vec4(lightColor, 1.0);
}
)"