#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out Data
{
	vec2 texcoord;
}vertexData;

void main()
{
	vertexData.texcoord = texcoord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}