#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};


enum class TextureType
{
	AMBIENT, DIFFUSE, SPECULAR
};

struct Texture
{
	GLuint id;
	std::string path;
	TextureType type;
};

class Mesh
{
private:
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

public:
	void draw(Shader shader);

};

#endif