#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out vec2 ftexcoord;

uniform mat4 model;

void main()
{
	ftexcoord = texcoord;
	gl_Position = model * vec4(position, 1.0f);
}