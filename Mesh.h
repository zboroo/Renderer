#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class Mesh
{
private:
	GLuint VAO{0};
	GLuint VBO{0};
	GLuint IBO{0};

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

public:
	Mesh(std::vector<Vertex> vertices);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

public:
	void bind();
	void draw(const Shader& shader);
	void clear();

private:
	void init(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
};

#endif