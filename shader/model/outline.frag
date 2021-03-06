#version 430 core

out vec4 color;

in Data
{
	vec2 texcoord;
}fragmentData;

struct Material
{
	sampler2D ambient0;
	sampler2D diffuse0;
	sampler2D specular0;
};

uniform Material material;

void main()
{
	color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}