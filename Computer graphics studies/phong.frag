R"(#version 330 core
layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    vec4 lightSource1;
    vec4 attenuation1;
    vec4 lightSource2;
    vec4 attenuation2;
};

out vec4 FragColor;
  
uniform sampler2D texture1;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;


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

void main()
{
    vec3 result1 = CalcLight(lightSource1, attenuation1);
    vec3 result2 = CalcLight(lightSource2, attenuation2);

    FragColor = vec4(result1 + result2 , 1.0);
}
)"