#version 430 core
out vec4 color;

in vec3 ftexcoord;

uniform samplerCube cubeMap;

void main()
{
	color = texture(cubeMap, ftexcoord);
}