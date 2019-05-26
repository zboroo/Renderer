#version 430 core

out vec4 color;

uniform vec3 cubeColor;
uniform vec3 lightColor;

uniform vec3 cameraPosition;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

uniform Material material;

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform PointLight pointLight;

in Data
{
	vec3 position;
	vec3 normal;
}fragmentData;

void main()
{
	vec3 ambient = pointLight.ambient * material.ambient;;

	vec3 lightDirection = normalize(fragmentData.position - pointLight.position);
	float diffuseStrength = max(dot(-lightDirection, fragmentData.normal), 0.0f);
	vec3 diffuse = pointLight.diffuse * diffuseStrength * material.diffuse;

	vec3 viewDirection = normalize(fragmentData.position - cameraPosition);
	vec3 halfway = normalize(-lightDirection - viewDirection);
	float specularStrength = pow(max(dot(halfway, fragmentData.normal), 0.0f), 256);
	vec3 specular = pointLight.specular *  specularStrength * material.specular;

	color = vec4(ambient + diffuse + specular, 1.0f);
}