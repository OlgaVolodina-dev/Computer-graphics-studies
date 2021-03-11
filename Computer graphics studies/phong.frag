R"(#version 420 core
layout(std140, binding = 2) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    vec4 lightSource1;
    vec4 attenuation1;
    vec4 lightSource2;
    vec4 attenuation2;
    mat4 lightSpaceMatrix;
    vec4 lightDir;
};

out vec4 FragColor;
  
layout(binding=0) uniform sampler2D texture1;
layout(binding=2) uniform sampler2D depthMap;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 FragLightPos;


vec3 CalcLight(vec4 lightSource, vec4 attenuation) {

    vec3 lightColor = vec3(texture(texture1, TexCoord));

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(lightSource) - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(cameraPos) - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    float distance    = length(vec3(lightSource) - FragPos);
    float attenuation_ = 1.0 / (attenuation.x + attenuation.y * distance + 
    		    attenuation.z * (distance * distance));
    vec3 result = attenuation_ * ( diffuse + ambient);
    return result;

}

vec3 CalcDirectionalLight(vec3 lightDir)
{
    vec3 lightColor = vec3(texture(texture1, TexCoord)); 
    vec3 norm = normalize(Normal);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(cameraPos) - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result =  diffuse + ambient;
    return result;
}

void main()
{
    vec3 result1 = CalcLight(lightSource1, attenuation1);
    vec3 result2 = CalcLight(lightSource2, attenuation2);
    vec3 result3 = 0.1 * CalcDirectionalLight(vec3(lightDir));

     vec3 lightDepth = FragLightPos.xyz / FragLightPos.w * 0.5 + 0.5;
    float shadows = (lightDepth.z - 0.005 > texture(depthMap, lightDepth.xy).r) ? 0.3 : 1.0;
    if (lightDepth.x < 0.0 || lightDepth.x > 1.0 || lightDepth.y < 0.0 || lightDepth.y > 1.0) {
       shadows = 1.0;
    }
        FragColor = vec4(shadows * (result1 + result2 + result3) , 1.0);
}
)"