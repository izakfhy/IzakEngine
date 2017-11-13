#version 330 core

//in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  
flat in int ObjID;


layout (location = 0) out vec4 color;
layout (location = 3) out int PixelID;

struct Material {

    sampler2D  diffuse;
    sampler2D  specular;
    float shininess;
}; 


struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;

	int active;

};

uniform sampler2D diffuse1;
uniform Material material;
uniform vec3 viewPos;
uniform DirectionalLight Dlight; 
uniform PointLight Plight;
uniform int num_pt_lights;

#define NR_POINT_LIGHTS 10 
uniform PointLight pointLights[NR_POINT_LIGHTS];

layout (std140) uniform LightBlock {
    PointLight lights[NR_POINT_LIGHTS];
};

vec3 CalculateBlinnPhong(vec3 Lambient, vec3 Ldiffuse, vec3 Lspecular,vec3 lightDir)
{
	//phong lighting equation
	//ambient
	float ambientStrength = 0.1;
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * Lambient;

    // diffuse 
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * texture(material.diffuse, TexCoords).rgb) * Ldiffuse;

	//specular, blinn phong

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize( lightDir + viewDir );

	float specAngle = max(dot(halfDir, norm), 0.0);
	float specular = pow(specAngle, material.shininess);
	vec3 finalspec = Lspecular * (specular * texture(material.specular, TexCoords).rgb);

	vec3 result = (ambient + diffuse + finalspec) ;
 

	return result;
}

float CalculateAttenuation(vec3 Lpos, vec3 fpos, float constant, float linear, float quad)
{

	float distance    = length(Lpos - fpos);
	float attenuation = 1.0 / (constant + linear * distance + 
    				quad * (distance * distance));   
	
	return attenuation;
}

vec3 CalculatePointLight(PointLight light)
{
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 plbp = CalculateBlinnPhong(light.ambient,light.diffuse,light.specular,lightDir);
	float atten = CalculateAttenuation(light.position,FragPos,light.constant,light.linear,light.quadratic);
	plbp *= atten;
	return plbp;
}

void main()
{
	//directional light blinn phong
	vec3 LDir = normalize(-Dlight.direction);
	vec3 blinnphong = CalculateBlinnPhong(Dlight.ambient,Dlight.diffuse,Dlight.specular,LDir);

	//point light equation
	for(int i =0; i < num_pt_lights; ++i)
	{
		if(lights[i].active == 0)
			continue;
		blinnphong += CalculatePointLight(lights[i]);
	}

	PixelID = ObjID;
	color = vec4(blinnphong, 1.0);

	//color = texture(diffuse1, TexCoords);
	

}