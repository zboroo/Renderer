#include "Texture.h"

Texture::Texture(Shader shader, const std::string& name, const std::string& path, GLint activeNum)
{
	load(shader, name, path, activeNum);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

void Texture::load(Shader shader, const std::string& name, const std::string& path, GLint activeNum)
{
	glGenTextures(1, &texture);
	glActiveTexture(activeNum);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	int width = 0, height = 0, channel = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, 0);



	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	shader.use();
	shader.setInt(name, activeNum - GL_TEXTURE0);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}
