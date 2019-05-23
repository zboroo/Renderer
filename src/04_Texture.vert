#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;

out Data
{
	vec3 position;
	vec3 color;
	vec2 texcoord;
}vertexData;

void main()
{
	vertexData.position = position;
	vertexData.color = color;
	vertexData.texcoord = texcoord;

	gl_Position = vec4(position, 1.0f);	
}