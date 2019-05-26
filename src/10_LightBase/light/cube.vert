#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out Data
{
	vec3 position;
	vec3 normal;
}vertexData;

void main()
{
	vertexData.position = position;
	vertexData.normal = mat3(transpose(inverse(model))) * normal;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}