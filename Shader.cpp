#include "Shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : program(0)
{
	load(vertexShaderPath, fragmentShaderPath);
}

void Shader::load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compile(vertexShader, vertexShaderPath, ShaderType::VERTEX);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compile(fragmentShader, fragmentShaderPath, ShaderType::FRAGMENT);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	check(program, ShaderType::PROGRAM);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2fv(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec3fv(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4fv(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setMat2fv(const std::string& name, const glm::mat2& value)
{
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3fv(const std::string& name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4fv(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	setMat4fv("model", model);
	setMat4fv("view", view);
	setMat4fv("projection", projection);
}

void Shader::compile(GLuint shader, const std::string& shaderPath, ShaderType type)
{
	std::ifstream file(shaderPath);

	if (!file.is_open())
	{
		std::cout << "failed to open fragment shader file" << std::endl;
		return;
	}

	std::string content;
	std::string line;

	while (std::getline(file, line))
	{
		content += line + '\n';
	}

	file.close();

	const char* source = content.c_str();
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	check(shader, type);
}

void Shader::check(GLuint id, ShaderType type)
{
	GLint success;
	char log[512];

	switch (type)
	{
	case ShaderType::VERTEX:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, log);
			std::cout << "failed compile vertex shader: " << log << std::endl;
		}
		break;
	case ShaderType::FRAGMENT:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 512, nullptr, log);
			std::cout << "failed compile fragment shader: " << log << std::endl;
		}
		break;
	case ShaderType::PROGRAM:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 1, nullptr, log);
			std::cout << "failed link shader program: " << log << std::endl;
		}
		break;
	default:
		break;
	}
}
