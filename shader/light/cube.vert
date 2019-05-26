#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out Data
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}vertexData;

void main()
{
	vertexData.position = vec3(model * vec4(position, 1.0f));
	vertexData.normal = mat3(transpose(inverse(model))) * normal;
	vertexData.texcoord = texcoord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}