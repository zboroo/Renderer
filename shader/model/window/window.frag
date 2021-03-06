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
uniform float runtime;

void main()
{
	vec4 texColor = texture(material.ambient0, fragmentData.texcoord);
    if(texColor.a < 0.1)
		discard;
    color = texColor;
}