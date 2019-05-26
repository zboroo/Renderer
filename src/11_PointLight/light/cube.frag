#version 430 core

out vec4 color;

uniform vec3 cubeColor;
uniform vec3 lightColor;

uniform vec3 cameraPosition;

struct Material
{
	sampler2D ambient;
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

uniform Material material;

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform PointLight pointLight;

in Data
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}fragmentData;

void main()
{
	float fdistance = length(pointLight.position - fragmentData.position);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear * fdistance + pointLight.quadratic * fdistance * fdistance);

	vec3 ambient = pointLight.ambient * texture(material.ambient, fragmentData.texcoord).rgb;
	ambient *= attenuation;

	vec3 lightDirection = normalize(fragmentData.position - pointLight.position);
	float diffuseStrength = max(dot(-lightDirection, fragmentData.normal), 0.0f);
	vec3 diffuse = pointLight.diffuse * diffuseStrength * texture(material.diffuse, fragmentData.texcoord).rgb;
	diffuse *= attenuation;

	vec3 viewDirection = normalize(fragmentData.position - cameraPosition);
	vec3 halfway = normalize(-lightDirection - viewDirection);
	float specularStrength = pow(max(dot(halfway, fragmentData.normal), 0.0f), 256);
	vec3 specular = pointLight.specular *  specularStrength * texture(material.specular, fragmentData.texcoord).rgb;
	specular *= attenuation;

	color = vec4(ambient + diffuse + specular, 1.0f);
}