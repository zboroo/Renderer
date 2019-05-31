#ifndef _MODEL_H_
#define _MODEL_H_

#include <iostream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "stb_image.h"
#include "Mesh.h"

class Model
{
private:
	std::vector<Mesh> meshs;
	std::string directory;
	std::vector<Texture> loadedTextures;

public:
	Model(const std::string &path);
	~Model();

private:
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> processMeshTexture(aiMaterial* material, aiTextureType type, TextureType textureType);

public:
	void draw(const Shader& shader);
};

#endif
