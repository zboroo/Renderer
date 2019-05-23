#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

GLFWwindow* window;
std::string title = "Renderer";
int width = 800, height = 800;

void processFramebufferSize(GLFWwindow* window, int width, int height);
void processKeyboard();

int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		std::cout << "failed to init GLFW" << std::endl;
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init GLAD" << std::endl;
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(window, processFramebufferSize);

	std::ifstream vertexShaderFile("shader/base.vert");
	if (!vertexShaderFile.is_open())
	{
		std::cout << "failed to open vertex shader file" << std::endl;
		return EXIT_FAILURE;
	}
	std::string vertexShaderContent;
	std::string line;
	while (std::getline(vertexShaderFile, line))
	{
		vertexShaderContent += line + '\n';
	}
	vertexShaderFile.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = vertexShaderContent.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		char log[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, log);
		std::cout << "failed compile vertex shader: " << log << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream fragmentShaderFile("shader/base.frag");
	if (!fragmentShaderFile.is_open())
	{
		std::cout << "failed to open fragment shader file" << std::endl;
		return EXIT_FAILURE;
	}
	std::string fragmentShaderContent;
	while (std::getline(fragmentShaderFile, line))
	{
		fragmentShaderContent += line + '\n';
	}
	fragmentShaderFile.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = fragmentShaderContent.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
		std::cout << "failed compile fragment shader: " << log << std::endl;
		return EXIT_FAILURE;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetProgramInfoLog(program, 1, nullptr, log);
		std::cout << "failed link shader program: " << log << std::endl;
		return EXIT_FAILURE;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] =
	{
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f
	};

	GLint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLint positionLocation = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

	GLint colorLocation = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		processKeyboard();

		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void processFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processKeyboard()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
