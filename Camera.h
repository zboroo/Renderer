#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraMovement
{
	FORWARD, BACKWARD, RIGHT, LEFT, UP, DOWN
};

class Camera
{
private:
	float cameraSpeed{ 8.0f };
	glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
	glm::vec3 cameraPosition{ 0.0f, 20.0f, 20.0f };
	glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };
	glm::vec3 cameraFront{ 0.0f, -glm::sqrt(2) / 2.0f, -glm::sqrt(2) / 2.0f };
	glm::vec3 cameraRight{ 1.0f, 0.0f, 0.0f };

	float cursorX{ 0.0f };
	float cursorY{ 0.0f };
	float mouseSensitivity{ 0.05f };
	bool  mouseFirstInput{ true };
	float yaw{ -90.0f };
	float pitch{ -45.0f };

	float fov{ 45.0f };

public:
	Camera() = default;

public:
	void processKeyboadMove(CameraMovement movement, float deltaTime);
	void processMouseMove(float x, float y);
	void processMouseScroll(float offset);

	glm::mat4 getViewMatrix();

	float getFOV()
	{
		return fov;
	}

	glm::vec3 getCameraPosition()
	{
		return cameraPosition;
	}

	glm::vec3 getCameraFront()
	{
		return cameraFront;
	}
};

#endif


