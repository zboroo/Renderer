#version 430 core

out vec4 color;

in Data
{
	vec3 position;
	vec2 texcoord;
}fragmentData;

struct Material
{
	sampler2D ambient0;
	sampler2D diffuse0;
	sampler2D specular0;
};

uniform Material material;
uniform int num;

void main()
{
	if(num == 0)
	{
		color = vec4(texture(material.diffuse0, fragmentData.texcoord).rgb, 1.0f);
	}
	if(num == 1)
	{
		color = vec4(fragmentData.position, 1.0f);
	}
	if(num == 2)
	{
		color = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z, 1.0f);
	}
}