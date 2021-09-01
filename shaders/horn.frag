R"(#version 440 core
layout(std140, binding = 0) uniform GlobalMatrices
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
layout(binding=1) uniform sampler2D metallicTexture;
layout(binding=2) uniform sampler2D depthMap1;
layout(binding=7) uniform sampler2D depthMap2;
layout(binding=8) uniform sampler2D depthMap3;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 FragLightPos1;
in vec4 FragLightPos2;
in vec4 FragLightPos3;


vec3 objectColor;
vec3 CalcLight(vec4 lightSource, vec4 attenuation, vec4 lightCol) {

    vec3 lightColor = lightCol.xyz;

    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength = 0.8;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(lightSource) - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  lightCol.xyz * diff * lightColor * diffuseStrength;

    float specularStrength = 0.5 + texture(metallicTexture, TexCoord).r;
    vec3 viewDir = normalize(vec3(cameraPos) - FragPos);
    vec3 reflectDir = reflect(normalize(-lightDir), norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 20);
    vec3 specular = specularStrength * spec * lightColor;  

    float distance    = length(vec3(lightSource) - FragPos);
    float attenuation_ = 1.0 / (attenuation.x + attenuation.y * distance + 
    		    attenuation.z * (distance * distance));

    vec3 result = attenuation_ * ( diffuse + ambient + specular) * objectColor;
    return result;

}

vec3 CalcDirectionalLight(vec3 lightDir)
{
    vec3 lightColor = vec3(2.0); 

    vec3 norm = normalize(Normal);
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;
 
    float diffuseStrength = 1.0;
    float diff = max(dot(norm, normalize(lightDir)), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;

    float specularStrength = 0.5 + texture(metallicTexture, TexCoord).r;
    vec3 viewDir = normalize(cameraPos.xyz - FragPos);
    vec3 reflectDir = normalize(reflect(normalize(-lightDir), norm));

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 20.0);
    vec3 specular =  specularStrength * spec * lightColor;  
    
    vec3 result =  ( ambient + diffuse + specular) * objectColor;
    return result;
}

float CalculateVSM(vec3 lightDepth, sampler2D depthMap)
{
    float shadows = 1.0;
    vec2 depthColor = textureLod(depthMap, lightDepth.xy, 0).xy;
    if (lightDepth.z - 0.005 > textureLod(depthMap, lightDepth.xy, 0).r) {
        float mu = depthColor.x;
        float sigma = depthColor.y;
        float var = max(sigma - pow(mu, 2.0), 0.0002);
        float distance = lightDepth.z - mu; 
        shadows = min(var / (var + pow(distance, 2.0)), 1.0);
    }
    return shadows;
}

float CalculatePCF(vec3 lightDepth, sampler2D depthMap)
{
    float shadows = 1.0;
    float bias = 0.005;
    if (lightDepth.z - bias > textureLod(depthMap, lightDepth.xy, 0).r) {
        shadows = 0.0;
        vec2 texelSize = 1.0 / textureSize(depthMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = textureLod(depthMap, lightDepth.xy + vec2(x, y) * texelSize, 0).r; 
                shadows += lightDepth.z - bias > pcfDepth ? 0.0 : 1.0;
            }    
        }
        shadows /= 9;
    }
    return shadows;
}


void main()
{
objectColor =  vec3(texture(texture1, TexCoord));
    vec3 result1 = CalcLight(lightSource1, attenuation1, lightColor1);
    vec3 result2 = CalcLight(lightSource2, attenuation2,  lightColor2);
    vec3 result3 =  CalcDirectionalLight(vec3(lightDir));
    float shadows = 1.0;
    vec3 lightDepth;
    lightDepth = FragLightPos1.xyz / FragLightPos1.w * 0.5 + 0.5;
    if (lightDepth.z < 1.0 && lightDepth.x > 0.001 && lightDepth.x < 0.99 && lightDepth.y > 0.001 && lightDepth.y < 0.99) {
        shadows = CalculateVSM(lightDepth, depthMap1);
        FragColor = vec4(((shadows * result3) + result2+ result1) / 3.0 , 1.0);
        return;
    } 

    lightDepth = FragLightPos2.xyz / FragLightPos2.w * 0.5 + 0.5;
    if (lightDepth.z < 1.0 && lightDepth.x > 0.001 && lightDepth.x < 0.99 && lightDepth.y > 0.001 && lightDepth.y < 0.99) {
        shadows = CalculatePCF(lightDepth, depthMap2);
        FragColor = vec4(((shadows * result3) + result2+ result1) / 3.0 , 1.0);
        return;
    } 

    lightDepth = FragLightPos3.xyz / FragLightPos3.w * 0.5 + 0.5;
    if (lightDepth.z < 1.0 && lightDepth.x > 0.001 && lightDepth.x < 0.99 && lightDepth.y > 0.001 && lightDepth.y < 0.99) {
        shadows = CalculatePCF(lightDepth, depthMap3);
        FragColor = vec4(((shadows * result3) + result2+ result1) / 3.0 , 1.0);
        return;
    } 


    FragColor = vec4(((shadows * result3) + result2+ result1) / 3.0 , 1.0);
    //FragColor = vec4(lightDepth.x);
}
)"