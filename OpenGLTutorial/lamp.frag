#version 330 core
out vec4 FragColor;
  
uniform vec4 objectColor;
uniform vec3 lightColor;

void main()
{
    FragColor = objectColor;
}