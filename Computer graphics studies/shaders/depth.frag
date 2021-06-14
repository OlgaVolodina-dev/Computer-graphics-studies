R"(#version 420 core
out vec4 FragColor;
  
void main()
{
    float z = gl_FragCoord.z;
    FragColor = vec4(z, z*z, z, 1.0);
}
)"