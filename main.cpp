#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "Shader.h"
#include"Camera.h"
#include "Model.h"

GLFWwindow* window;
std::string title = "Renderer";
int width = 1920, height = 1080;

float deltaTime = 0.0f;
float prevTime  = 0.0f;

Camera camera;

void calculateFPS();
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

	/* 坐标系 */
	Shader axisShader("shader/axis.vert", "shader/axis.frag");

	std::vector<glm::vec3> axis;
	for (int x = -10; x <= 10; x++)
	{
		axis.push_back(glm::vec3(x, 0.0f, -10.0f));
		axis.push_back(glm::vec3(x, 0.0f, 10.0f));
	}
	for (int z = -10; z <= 10; z++)
	{
		axis.push_back(glm::vec3(-10.0f, 0.0f, z));
		axis.push_back(glm::vec3(10.0f, 0.0f, z));
	}

	GLuint axisVAO;
	glGenVertexArrays(1, &axisVAO);
	glBindVertexArray(axisVAO);

	GLuint axisVBO;
	glGenBuffers(1, &axisVBO);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* axis.size(), axis.data(), GL_STATIC_DRAW);

	GLint axisPositionLocation = glGetAttribLocation(axisShader.program, "position");
	glEnableVertexAttribArray(axisPositionLocation);
	glVertexAttribPointer(axisPositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	GLfloat triangle[] =
	{
		10.0f, 0.0f,  0.0f,
		 9.7f, 0.0f,  0.1f,
		 9.7f, 0.0f, -0.1f,

		 0.0f, 0.0f,  10.0f,
		 0.1f, 0.0f,  9.7f,
		-0.1f, 0.0f,  9.7f
	};

	GLuint triangleVAO;
	glGenVertexArrays(1, &triangleVAO);
	glBindVertexArray(triangleVAO);

	GLuint triangleVBO;
	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	GLint trianglePositionLocation = glGetAttribLocation(axisShader.program, "position");
	glEnableVertexAttribArray(trianglePositionLocation);
	glVertexAttribPointer(trianglePositionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	Model sponzaModel("model/man/man03.obj");
	Shader sponzaShader("shader/model/sponza/sponza.vert", "shader/model/sponza/sponza.frag");

	glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		calculateFPS();
		processKeyboard();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

		/* 绘制坐标系 */
		axisShader.use();
		glBindVertexArray(axisVAO);
		axisShader.setMVP(model, view, projection);
		
		for (int i = 0; i < 42; i++)
		{
			if (i == 10)
			{
				axisShader.setVec3fv("axisColor", glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else if (i == 31)
			{
				axisShader.setVec3fv("axisColor", glm::vec3(1.0f, 0.0f, 0.0f));
			}
			else
			{
				axisShader.setVec3fv("axisColor", glm::vec3(0.26f, 0.26f, 0.26f));
			}
			glDrawArrays(GL_LINES, i*2, 2);
		}

		glBindVertexArray(triangleVAO);
		axisShader.setMVP(model, view, projection);
		axisShader.setVec3fv("axisColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		axisShader.setVec3fv("axisColor", glm::vec3(0.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 3, 3);
		
		sponzaShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f));
		sponzaShader.setMVP(model, view, projection);
		sponzaShader.setVec3fv("cameraPosition", camera.getCameraPosition());
		sponzaModel.draw(sponzaShader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &axisVAO);
	glDeleteBuffers(1, &axisVBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void calculateFPS()
{
	float currentTime = static_cast<float>(glfwGetTime());
	deltaTime = currentTime - prevTime;
	prevTime = currentTime;
}

void processFramebufferSize(GLFWwindow* window, int width, int height)
{
	::width = width;
	::height = height;
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
		camera.processKeyboadMove(CameraMovement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboadMove(CameraMovement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboadMove(CameraMovement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboadMove(CameraMovement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.processKeyboadMove(CameraMovement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.processKeyboadMove(CameraMovement::DOWN, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void processCursor(GLFWwindow* window, double x, double y)
{
	camera.processMouseMove(static_cast<float>(x), static_cast<float>(y));
}

void processScroll(GLFWwindow*, double offsetX, double offsetY)
{
	camera.processMouseScroll(static_cast<float>(offsetY));
}
