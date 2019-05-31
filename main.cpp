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
	Model sponzaModel("model/sponza/sponza.obj");
	Shader sponzaShader("shader/model/sponza/sponza.vert", "shader/model/sponza/sponza.frag");

	float skyboxVertices[] = {
		// positions          
		-30.0f,  30.0f, -30.0f,
		-30.0f, -30.0f, -30.0f,
		 30.0f, -30.0f, -30.0f,
		 30.0f, -30.0f, -30.0f,
		 30.0f,  30.0f, -30.0f,
		-30.0f,  30.0f, -30.0f,

		-30.0f, -30.0f,  30.0f,
		-30.0f, -30.0f, -30.0f,
		-30.0f,  30.0f, -30.0f,
		-30.0f,  30.0f, -30.0f,
		-30.0f,  30.0f,  30.0f,
		-30.0f, -30.0f,  30.0f,

		 30.0f, -30.0f, -30.0f,
		 30.0f, -30.0f,  30.0f,
		 30.0f,  30.0f,  30.0f,
		 30.0f,  30.0f,  30.0f,
		 30.0f,  30.0f, -30.0f,
		 30.0f, -30.0f, -30.0f,

		-30.0f, -30.0f,  30.0f,
		-30.0f,  30.0f,  30.0f,
		 30.0f,  30.0f,  30.0f,
		 30.0f,  30.0f,  30.0f,
		 30.0f, -30.0f,  30.0f,
		-30.0f, -30.0f,  30.0f,

		-30.0f,  30.0f, -30.0f,
		 30.0f,  30.0f, -30.0f,
		 30.0f,  30.0f,  30.0f,
		 30.0f,  30.0f,  30.0f,
		-30.0f,  30.0f,  30.0f,
		-30.0f,  30.0f, -30.0f,

		-30.0f, -30.0f, -30.0f,
		-30.0f, -30.0f,  30.0f,
		 30.0f, -30.0f, -30.0f,
		 30.0f, -30.0f, -30.0f,
		-30.0f, -30.0f,  30.0f,
		 30.0f, -30.0f,  30.0f
	};
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// skybox shader
	Shader skyboxShader("shader/skybox.vert", "shader/skybox.frag");

	// Cube Map
	GLuint cubeMapTexture;
	glGenTextures(1, &cubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	glActiveTexture(GL_TEXTURE0);

	std::vector<std::string> skyboxPath =
	{
		"texture/skybox/right.jpg", "texture/skybox/left.jpg", "texture/skybox/top.jpg",
		"texture/skybox/bottom.jpg", "texture/skybox/front.jpg", "texture/skybox/back.jpg"
	};

	int width = 0, height = 0, channel = 0;
	unsigned char* data;
	for (size_t i = 0; i < skyboxPath.size(); i++)
	{
		data = stbi_load(skyboxPath[i].c_str(), &width, &height, &channel, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}

	skyboxShader.use();
	skyboxShader.setInt("cubeMap", 0);

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

		

		// draw man
		manShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		manShader.setMVP(model, view, projection);
		manShader.setInt("num", 0);
		manModel.draw(manShader);

		// draw sponza
		sponzaShader.use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.01f));
		sponzaShader.setMVP(model, view, projection);
		sponzaModel.draw(sponzaShader);

		// draw skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		skyboxShader.setMat4fv("view", view);
		skyboxShader.setMat4fv("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

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
