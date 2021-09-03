R"(#version 440 core
layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 view;
    mat4 projection;
    vec4 cameraPos;
    mat4 lightSpaceMatrix[3];
    vec4 lightDir;
};

out vec4 FragColor;
  
layout(binding=0) uniform sampler2D texture1;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

vec3 objectColor;

vec3 CalcDirectionalLight(vec3 lightDir)
{
    vec3 lightColor = vec3(1.0); 
    vec3 norm = normalize(Normal);
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;
 
    float diffuseStrength = 1.0;
    float diff = max(dot(norm, normalize(lightDir)), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(cameraPos) - FragPos);
    vec3 reflectDir = reflect(normalize(-lightDir), norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result =  (diffuse + ambient + specularStrength ) * objectColor;
    return result;
}

void main()
{
    objectColor =  vec3(texture(texture1, TexCoord));
    vec3 result =  CalcDirectionalLight(vec3(lightDir));

    FragColor = vec4(result, 1.0);
}
)"