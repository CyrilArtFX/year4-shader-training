#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emissive;
	float shininess;
};

struct Light
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 tFragPos;
in vec3 tNormal;
in vec2 tTexCoord;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	//  ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, tTexCoord));

	//  diffuse
	vec3 norm = normalize(tNormal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, tTexCoord));

	//  specular
	vec3 viewDir = normalize(viewPos - tFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, tTexCoord));

	//  emissive
	vec3 emissive = texture(material.emissive, tTexCoord).rgb;

	//  result
	vec3 lightingResult = ambient + diffuse + specular + emissive;
	FragColor = vec4(lightingResult, 1.0f);
}