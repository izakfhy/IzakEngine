#version 330 core
  
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
layout (location = 1) in vec3 norm;


//out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;  
flat out int ObjID;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 InverseTransposeMat;
uniform int objectid;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
  //ourColor = color;
    ObjID = objectid;
	Normal = mat3(InverseTransposeMat)* norm;
	FragPos = vec3(model * vec4(position, 1.0));
    TexCoords = texCoord;
}