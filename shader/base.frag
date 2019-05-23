#version 430 core

out vec4 color;

in Data
{
	vec3 position;
	vec3 color;
	vec2 texcoord;
}fragmentData;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float runtime;

void main()
{
	color = mix(texture(texture0, fragmentData.texcoord), texture(texture1, fragmentData.texcoord), abs(sin(runtime)));;
}