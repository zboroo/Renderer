#version 430 core

out vec4 color;
in vec2 ftexcoord;

uniform sampler2D screenTexture;

const float offset = 1/300.0f;

void main()
{
	// ∑¥œ‡
	// color = vec4(1.0f - texture(screenTexture, ftexcoord).rgb, 1.0f);
	// ª“∂»
	// vec3 textureColor = texture(screenTexture, ftexcoord).rgb;
	// float avg = 0.2126*textureColor.r + 0.7152*textureColor.g + 0.0722*textureColor.b;
	// color = vec4(avg, avg, avg, 1.0f);
	
	vec2 offsets[9] = 
	{
		vec2(-offset, offset),
		vec2(0.0f, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(offset, 0.0f),
		vec2(-offset, -offset),
		vec2(0.0f, -offset),
		vec2(offset, -offset)
	};

	//float kernal[9] = 
	//{
	//	-1, -1, -1,
    //    -1,  9, -1,
    //    -1, -1, -1
	//};

	float kernal[9] = 
	{
		1.0f/16, 2.0f/16, 1.0f/16,
		2.0f/16, 4.0f/16, 2.0f/16,
		1.0f/16, 2.0f/16, 1.0f/16
	};

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = texture(screenTexture, ftexcoord.st + offsets[i]).rgb;
	}

	vec3 col = vec3(0.0f);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernal[i];

	color = vec4(col, 1.0f);
}