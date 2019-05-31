#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include"Camera.h"
#include "Model.h"

GLFWwindow* window;
std::string title = "Renderer";
int width = 1920, height = 1080;

float deltaTime = 0.0f;
float prevTime  = 0.0f;

Camera camera;

bool init();
void calculateFPS();
void processFramebufferSize(GLFWwindow* window, int width, int height);
void processKeyboard();
void processCursor(GLFWwindow* window, double x, double y);
void processScroll(GLFWwindow*, double offsetX, double offsetY);

int main(int argc, char** argv)
{
	if (!init())
		return EXIT_FAILURE;

	// Axis Mesh
	Shader axisShader("shader/axis.vert", "shader/axis.frag");

	std::vector<Vertex> axisVertices;

	for (int x = -10; x <= 10; x++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(x, 0.0f, -10.0f);
		axisVertices.push_back(vertex);
		vertex.position = glm::vec3(x, 0.0f, 10.0f);
		axisVertices.push_back(vertex);
	}
	for (int z = -10; z <= 10; z++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(-10.0f, 0.0f, z);
		axisVertices.push_back(vertex);
		vertex.position = glm::vec3(10.0f, 0.0f, z);
		axisVertices.push_back(vertex);
	}

	Mesh axisMesh(axisVertices);

	// RGB Triangle Mesh
	std::vector<Vertex> redTriangleVertices;
	redTriangleVertices.push_back(Vertex{ glm::vec3(10.0f, 0.0f, 0.0f) });
	redTriangleVertices.push_back(Vertex{ glm::vec3(9.7f, 0.0f, 0.1f) });
	redTriangleVertices.push_back(Vertex{ glm::vec3(9.7f, 0.0f, -0.1f) });
	Mesh redTriangleMesh(redTriangleVertices);

	std::vector<Vertex> blueTriangleVertices;
	blueTriangleVertices.push_back(Vertex{ glm::vec3(0.0f, 0.0f, 10.0f) });
	blueTriangleVertices.push_back(Vertex{ glm::vec3(0.1f, 0.0f, 9.7f) });
	blueTriangleVertices.push_back(Vertex{ glm::vec3(-0.1f, 0.0f, 9.7f) });
	Mesh blueTriangleMesh(blueTriangleVertices);

	// Man Mesh
	Model manModel("model/man/man03.obj");
	Shader manShader("shader/model/man/man.vert", "shader/model/man/man.frag");

	// Sponza Mesh
	//Model sponzaModel("model/sponza/sponza.obj");
	//Shader sponzaShader("shader/model/sponza/sponza.vert", "shader/model/sponza/sponza.frag");

	GLfloat screenVertices[] =
	{
		-0.7f,  -0.7f, 0.0f, 1.0f, 1.0f,
		-0.7f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,

		-0.7f,  -0.7f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -0.7f, 0.0f, 0.0f, 1.0f
	};

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);

	GLuint screenVBO;
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

	Shader screenShader("shader/screen.vert", "shader/screen.frag");
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	// FBO1
	GLuint FBO1;
	glGenFramebuffers(1, &FBO1);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO1);

	GLuint colorAttachment1;
	glGenTextures(1, &colorAttachment1);
	glBindTexture(GL_TEXTURE_2D, colorAttachment1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment1, 0);

	GLuint stencilDepthRenderBuffer1;
	glGenRenderbuffers(1, &stencilDepthRenderBuffer1);
	glBindRenderbuffer(GL_RENDERBUFFER, stencilDepthRenderBuffer1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilDepthRenderBuffer1);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "frame buffer not complete" << std::endl;

	// FBO2
	GLuint FBO2;
	glGenFramebuffers(1, &FBO2);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO2);

	GLuint colorAttachment2;
	glGenTextures(1, &colorAttachment2);
	glBindTexture(GL_TEXTURE_2D, colorAttachment2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment2, 0);

	GLuint stencilDepthRenderBuffer2;
	glGenRenderbuffers(1, &stencilDepthRenderBuffer2);
	glBindRenderbuffer(GL_RENDERBUFFER, stencilDepthRenderBuffer2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilDepthRenderBuffer2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "frame buffer not complete" << std::endl;

	glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
	
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		calculateFPS();
		processKeyboard();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
		
		// draw axis
		axisShader.use();
		axisMesh.bind();
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

		axisShader.setVec3fv("axisColor", glm::vec3(1.0f, 0.0f, 0.0f));
		redTriangleMesh.draw(axisShader);
		axisShader.setVec3fv("axisColor", glm::vec3(0.0f, 0.0f, 1.0f));
		blueTriangleMesh.draw(axisShader);

		// start framebuffer PASS 1
		glBindFramebuffer(GL_FRAMEBUFFER, FBO1);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw man
		manShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		manShader.setMVP(model, view, projection);
		manShader.setInt("num", 0);
		manModel.draw(manShader);

		// start framebuffer PASS 2
		glBindFramebuffer(GL_FRAMEBUFFER, FBO2);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw man
		manShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		manShader.setMVP(model, view, projection);
		manShader.setInt("num", 0);
		manModel.draw(manShader);

		// end framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.23f, 0.23f, 0.23f, 1.0f);

		// draw man
		manShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		manShader.setMVP(model, view, projection);
		manShader.setInt("num", 0);
		manModel.draw(manShader);

		// draw screen
		screenShader.use();
		glBindVertexArray(screenVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorAttachment1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, 0.2f, 0.0f));
		screenShader.setMat4fv("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		screenShader.use();
		glBindVertexArray(screenVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorAttachment2);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.5f, 0.2f, 0.0f));
		screenShader.setMat4fv("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// draw sponza
		//sponzaShader.use();
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.01f));
		//sponzaShader.setMVP(model, view, projection);
		//sponzaModel.draw(sponzaShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}

bool init()
{
	if (!glfwInit())
	{
		std::cout << "failed to init GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to init GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(window, processFramebufferSize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, processCursor);
	glfwSetScrollCallback(window, processScroll);

	return true;
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
	/*if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.processKeyboadMove(CameraMovement::DOWN, deltaTime);
	}*/

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
