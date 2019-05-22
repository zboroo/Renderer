#version 430 core

out vec4 color;

in Data
{
	vec3 position;
	vec3 color;
}fragmentData;

void main()
{
	color = vec4(fragmentData.color, 1.0f);
}