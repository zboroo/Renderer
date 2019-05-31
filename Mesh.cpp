#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices) : vertices(vertices)
{
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	init(vertices, indices, textures);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices)
{
	std::vector<Texture> textures;
	init(vertices, indices, textures);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) : vertices(vertices), indices(indices), textures(textures)
{
	init(vertices, indices, textures);
}

void Mesh::bind()
{
	glBindVertexArray(VAO);
}

void Mesh::draw(const Shader& shader)
{
	glBindVertexArray(VAO);

	unsigned ambientNum = 0;
	unsigned diffuseNum = 0;
	unsigned specularNum = 0;
	
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		Texture texture = textures[i];
		std::string name("material.");

		if (TextureType::AMBIENT == texture.type)
		{
			name += "ambient" + std::to_string(ambientNum);
			ambientNum++;
		}

		if (TextureType::DIFFUSE == texture.type)
		{
			name += "diffuse" + std::to_string(diffuseNum);
			diffuseNum++;
		}

		if (TextureType::SPECULAR == texture.type)
		{
			name += "specular" + std::to_string(specularNum);
			specularNum++;
		}

		shader.setInt(name, i);
		glBindTexture(GL_TEXTURE_2D, texture.id);
	}
	
	if (indices.size() > 0)
	{
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
	}
	
}

void Mesh::clear()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);

	for (size_t i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures[i].id);
	}
}

void Mesh::init(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
}
