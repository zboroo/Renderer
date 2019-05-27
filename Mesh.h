#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
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

	Shader shader;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, Shader shader);
	~Mesh();
};

#endif