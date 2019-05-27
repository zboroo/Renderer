#version 430 core

out vec4 color;

in Data
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}fragmentData;

struct Material
{
	sampler2D ambient;
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

struct DirectionLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

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

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectionLight directionLight;
#define POINT_LIGHT_NUM 3
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLight;

vec3 calcDriectionLight(DirectionLight directionLight, vec3 normal, vec3 viewDirection);
vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDirection);
vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDirection);

uniform bool startPointLight;
uniform bool startSpotLight;

void main()
{
	vec3 normal = normalize(fragmentData.normal);
	vec3 viewDirection = normalize(fragmentData.position - cameraPosition);

	vec3 result = calcDriectionLight(directionLight, normal, viewDirection);

	if(startPointLight)
	{
		for(int i = 0; i < POINT_LIGHT_NUM; i++)
		{
		 	result += calcPointLight(pointLights[i], normal, viewDirection);
		}
	}
	
	if(startSpotLight)
	{
		result += calcSpotLight(spotLight, normal, viewDirection);
	}

	color = vec4(result, 1.0f);
}

vec3 calcDriectionLight(DirectionLight directionLight, vec3 normal, vec3 viewDirection)
{
	vec3 ambient = directionLight.ambient * texture(material.ambient, fragmentData.texcoord).rgb;

	float diffuseStrength = max(dot(normalize(-directionLight.direction), normal), 0.0f);
	vec3 diffuse = directionLight.diffuse * diffuseStrength * texture(material.diffuse, fragmentData.texcoord).rgb;

	vec3 halfway = normalize(normalize(-directionLight.direction) - viewDirection);
	float specularStrength = pow(max(dot(halfway, fragmentData.normal), 0.0f), material.shininess);
	vec3 specular = directionLight.specular * specularStrength * texture(material.specular, fragmentData.texcoord).rgb;
	
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 viewDirection)
{
	vec3 ambient = pointLight.ambient * texture(material.ambient, fragmentData.texcoord).rgb;

	vec3 lightDirection = normalize(fragmentData.position - pointLight.position);
	float diffuseStrength = max(dot(-lightDirection, normal), 0.0f);
	vec3 diffuse = pointLight.diffuse * diffuseStrength * texture(material.diffuse, fragmentData.texcoord).rgb;

	vec3 halfway = normalize(-lightDirection -viewDirection);
	float specularStrength = pow(max(dot(halfway, normal), 0.0f), material.shininess);
	vec3 specular = pointLight.specular * specularStrength * texture(material.specular, fragmentData.texcoord).rgb;

	float pixel_distance = length(fragmentData.position - pointLight.position);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear * pixel_distance + pointLight.quadratic * pixel_distance * pixel_distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(fragmentData.position - spotLight.position);
	float theta = dot(-lightDirection, normalize(-spotLight.direction));

	vec3 ambient = spotLight.ambient * texture(material.ambient, fragmentData.texcoord).rgb;

	float diffuseStrength = max(dot(lightDirection, normal), 0.0f);
	vec3 diffuse = spotLight.diffuse * texture(material.diffuse, fragmentData.texcoord).rgb;

	vec3 halfway = normalize(-viewDirection -lightDirection);
	float specularStrength = pow(max(dot(halfway, normal), 0.0f), material.shininess);
	vec3 specular = spotLight.specular * specularStrength * texture(material.specular, fragmentData.texcoord).rgb;

	float epsilon = spotLight.cutOff - spotLight.outCutOff;
	float intensity = clamp((theta - spotLight.outCutOff)/epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	float pixel_distance = length(fragmentData.position - spotLight.position);
	float attenuation = 1.0f / (spotLight.constant + spotLight.linear * pixel_distance + spotLight.quadratic * pixel_distance * pixel_distance);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}