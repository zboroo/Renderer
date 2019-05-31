#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "stb_image.h"

enum class TextureType
{
	AMBIENT, DIFFUSE, SPECULAR
};

class Texture
{
public:
	GLuint id{ 0 };
	std::string path{ "" };
	TextureType type{ TextureType::DIFFUSE };

public:
	Texture(const std::string& path, TextureType type);

};

#endif