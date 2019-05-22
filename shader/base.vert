#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out Data
{
	vec3 position;
	vec3 color;
}vertexData;

void main()
{
	vertexData.position = position;
	vertexData.color = color;

	gl_Position = vec4(position, 1.0f);	
}