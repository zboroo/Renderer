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
	sampler2D ambient0;
	sampler2D diffuse0;
	sampler2D specular0;
};

uniform Material material;

uniform vec3 cameraPosition;

void main()
{
	vec3 viewDirection = normalize(fragmentData.position - cameraPosition);
	float edge = dot(normalize(fragmentData.normal), -viewDirection);

	color = vec4(texture(material.diffuse0, fragmentData.texcoord).rgb, 1.0f);
}