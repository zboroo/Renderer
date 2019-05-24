#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"

GLFWwindow* window;
std::string title = "Renderer";
int width = 800, height = 800;

float deltaTime = 0.0f;
float prevTime  = 0.0f;

float cameraSpeed = 5.0f;
glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraRight(1.0f, 0.0f, 0.0f);

float cursorX		   = 0.0f;
float cursorY		   = 0.0f;
float mouseSensitivity = 0.05f;
bool  mouseFirstInput  = true;
float yaw			   = -90.0f;
float pitch			   = 0.0f;

float fov			   = 45.0f;

void processFramebufferSize(GLFWwindow* window, int width, int height);
void processKeyboard();
void processCursor(GLFWwindow* window, double x, double y);
void processScroll(GLFWwindow*, double offsetX, double offsetY);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, processCursor);
	glfwSetScrollCallback(window, processScroll);

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

	Texture texture0(baseShader, "texture0", "texture/container2.png", GL_TEXTURE0);
	Texture texture1(baseShader, "texture1", "texture/awesomeface.png", GL_TEXTURE1);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		processKeyboard();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		baseShader.use();
		glBindVertexArray(VAO);
		texture0.bind();
		texture1.bind();
		baseShader.setFloat("runtime", static_cast<float>(glfwGetTime()));

		glm::mat4 model(1.0f);
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 0.0f, 0.0f));

		glm::mat4 view(1.0f);
		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
		
		baseShader.setMat4fv("model", model);
		baseShader.setMat4fv("view", view);
		baseShader.setMat4fv("projection", projection);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);

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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPosition += cameraFront * cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPosition -= cameraFront * cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	}
}

void processCursor(GLFWwindow* window, double x, double y)
{
	if (mouseFirstInput)
	{
		cursorX = static_cast<float>(x);
		cursorY = static_cast<float>(y);
		mouseFirstInput = false;
	}

	float offsetX = static_cast<float>(x) - cursorX;
	float offsetY = cursorY - static_cast<float>(y);

	cursorX = static_cast<float>(x);
	cursorY = static_cast<float>(y);

	offsetX *= mouseSensitivity;
	offsetY *= mouseSensitivity;

	yaw += offsetX;
	pitch += offsetY;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if(pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front(1.0f);
	front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));

	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void processScroll(GLFWwindow*, double offsetX, double offsetY)
{
	if (fov >= 1.0f && fov <= 90.0f)
		fov -= offsetY;
	
	if (fov < 1.0f)
		fov = 1.0f;

	if (fov > 90.0f)
		fov = 90.0f;
}
