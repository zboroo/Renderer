#version 430 core

out vec4 color;

in Data
{
	vec2 texcoord;
}fragmentData;

struct Material
{
	sampler2D ambient0;
	sampler2D diffuse0;
	sampler2D specular0;
};

uniform Material material;

void main()
{
	//color = vec4(vec3(gl_FragCoord.z), 1.0f);
	color = vec4(texture(material.diffuse0, fragmentData.texcoord).rgb, 1.0f);
}