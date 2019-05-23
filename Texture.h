#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <string>
#include "glad/glad.h"
#include "stb_image.h"
#include "Shader.h"

class Texture
{
private:
	GLuint texture;

	GLint wrapS;
	GLint wrapT;

	GLint minFilter;
	GLint magFilter;

public:
	Texture() = default;
	Texture(Shader shader, const std::string& name, const std::string& path, GLint activeNum);
	~Texture();

public:
	void load(Shader shader, const std::string & name, const std::string& path, GLint activeNum);
	void bind();
};

#endif