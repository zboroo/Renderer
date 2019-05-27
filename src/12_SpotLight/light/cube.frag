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

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outCutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform SpotLight spotLight;

in Data
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}fragmentData;

void main()
{
	vec3 lightDirection = normalize(fragmentData.position - spotLight.position);
	float theta = dot(-lightDirection, normalize(-spotLight.direction));

	vec3 ambient = spotLight.ambient * texture(material.ambient, fragmentData.texcoord).rgb;

	float diffuseStrength = max(dot(lightDirection, normalize(fragmentData.normal)), 0.0f);
	vec3 diffuse = spotLight.diffuse * texture(material.diffuse, fragmentData.texcoord).rgb;

	vec3 viewDirection = normalize(fragmentData.position - cameraPosition);
	vec3 halfway = normalize(-viewDirection -lightDirection);
	float specularStrength = pow(max(dot(halfway, normalize(fragmentData.normal)), 0.0f), material.shininess);
	vec3 specular = spotLight.specular * specularStrength;

	float epsilon = spotLight.cutOff - spotLight.outCutOff;
	float intensity = clamp((theta - spotLight.outCutOff)/epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	float pixel_distance = length(fragmentData.position - spotLight.position);
	float attenuation = 1.0f / (spotLight.constant + spotLight.linear * pixel_distance + spotLight.quadratic * pixel_distance * pixel_distance);
	// ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	color = vec4(ambient + diffuse + specular, 1.0f);
}