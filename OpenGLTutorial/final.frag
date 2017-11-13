#version 330 core

noperspective in vec2 texCoord;

out vec3 color;

uniform sampler2D renderedTexture;

void main(){

   color = texture(renderedTexture, texCoord).xyz;
}