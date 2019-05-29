#include "Model.h"

Model::Model(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene)
	{
		std::cout << "failed to load mesh by assimp: " << path << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshs.push_back(processMesh(mesh, scene));
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		if (mesh->HasPositions())
		{
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;
		}
		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			vertex.bitangent.x = mesh->mBitangents[i].x;
			vertex.bitangent.y = mesh->mBitangents[i].y;
			vertex.bitangent.z = mesh->mBitangents[i].z;
		}

		vertices.push_back(vertex);
	}

	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	
	std::vector<Texture> ambientTextures = processMeshTexture(material, aiTextureType_AMBIENT, TextureType::AMBIENT);
	textures.insert(textures.end(), ambientTextures.begin(), ambientTextures.end());

	std::vector<Texture> diffuseTextures = processMeshTexture(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	std::vector<Texture> specularTextures = processMeshTexture(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
	textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
	

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::processMeshTexture(aiMaterial* material, aiTextureType type, TextureType textureType)
{
	std::vector<Texture> textures;

	unsigned textureNum = material->GetTextureCount(type);

	bool skip = false;

	for (unsigned i = 0; i < textureNum; i++)
	{
		aiString path;
		material->GetTexture(type, i, &path);

		for (unsigned j = 0; j < loadedTextures.size(); j++)
		{
			std::string pathTmp(path.C_Str());
			if (loadedTextures[j].path == pathTmp)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.type = textureType;
			texture.path = path.C_Str();

			glGenTextures(1, &texture.id);
			glBindTexture(GL_TEXTURE_2D, texture.id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int width = 0, height = 0, channel = 0;
			std::string texturePath = directory + "/" + path.C_Str();

			unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channel, 0);

			if (data)
			{
				GLenum format = GL_RGB;
				if (channel == 1)
					format = GL_RED;
				else if (channel == 3)
					format = GL_RGB;
				else if (channel == 4)
					format = GL_RGBA;

				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				stbi_image_free(data);
				textures.push_back(texture);
				loadedTextures.push_back(texture);
			}
			else
			{
				std::cout << "failed to load texture: " << texturePath << std::endl;
			}
		}
	}

	return textures;
}

void Model::draw(Shader shader)
{
	for (unsigned i = 0; i < meshs.size(); i++)
		meshs[i].draw(shader);
}