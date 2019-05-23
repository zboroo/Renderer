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
	GLuint program;

public:
	Shader() = default;
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

public:
	void load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void use();

	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setBool(const std::string& name, bool value);

	void setVec2fv(const std::string& name, const glm::vec2& value);
	void setVec3fv(const std::string& name, const glm::vec3& value);
	void setVec4fv(const std::string& name, const glm::vec4& value);

	void setMat2fv(const std::string& name, const glm::mat2& value);
	void setMat3fv(const std::string& name, const glm::mat3& value);
	void setMat4fv(const std::string& name, const glm::mat4& value);

private:
	void compile(GLuint shader, const std::string& shaderPath, ShaderType type);
	void check(GLuint id, ShaderType type);
};

#endif


