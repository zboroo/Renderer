#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "Shader.h"

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

	Shader baseShader("shader/base.vert", "shader/base.frag");

	GLfloat vertices[] =
	{
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
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

	GLint positionLocation = glGetAttribLocation(baseShader.program, "position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

	GLint colorLocation = glGetAttribLocation(baseShader.program, "color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texcoordLocation = glGetAttribLocation(baseShader.program, "texcoord");
	glEnableVertexAttribArray(texcoordLocation);
	glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint texture0;
	{
		glGenTextures(1, &texture0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width = 0, height = 0, channel = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("texture/container2.png", &width, &height, &channel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		baseShader.use();
		baseShader.setInt("texture0", 0);
	}

	GLuint texture1;
	{
		glGenTextures(1, &texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width = 0, height = 0, channel = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("texture/awesomeface.png", &width, &height, &channel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		baseShader.use();
		baseShader.setInt("texture1", 1);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		processKeyboard();

		baseShader.use();
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		baseShader.setFloat("runtime", static_cast<float>(glfwGetTime()));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

	glDeleteTextures(1, &texture0);
	glDeleteTextures(1, &texture1);

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
