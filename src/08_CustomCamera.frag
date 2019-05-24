#version 430 core

out vec4 color;

in Data
{
	vec3 position;
	vec2 texcoord;
}fragmentData;

uniform sampler2D texture0;

uniform float runtime;

void main()
{
	color = texture(texture0, fragmentData.texcoord);
}