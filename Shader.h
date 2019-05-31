#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"
#include "glm/glm.hpp"

enum class ShaderType
{
	VERTEX, FRAGMENT, PROGRAM
};

class Shader
{
public:
	GLuint program{0};

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

public:
	void use() const;

	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setBool(const std::string& name, bool value) const;

	void setVec2fv(const std::string& name, const glm::vec2& value) const;
	void setVec3fv(const std::string& name, const glm::vec3& value) const;
	void setVec4fv(const std::string& name, const glm::vec4& value) const;

	void setMat2fv(const std::string& name, const glm::mat2& value) const;
	void setMat3fv(const std::string& name, const glm::mat3& value) const;
	void setMat4fv(const std::string& name, const glm::mat4& value) const;

	void setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;

private:
	void compile(GLuint shader, const std::string& shaderPath, ShaderType type);
	void check(GLuint id, ShaderType type);
};

#endif


