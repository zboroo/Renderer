#version 430 core

out vec4 color;
uniform vec3 axisColor;

void main()
{
	color = vec4(axisColor, 1.0f);
}