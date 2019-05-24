#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out Data
{
	vec3 position;
	vec3 color;
}vertexData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vertexData.color = color;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}