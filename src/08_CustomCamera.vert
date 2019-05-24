#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out Data
{
	vec3 position;
	vec2 texcoord;
}vertexData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vertexData.position = position;
	vertexData.texcoord = texcoord;

	gl_Position = projection * view * model * vec4(position, 1.0f);	
}