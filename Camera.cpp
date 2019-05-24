#include "Camera.h"

void Camera::processKeyboadMove(CameraMovement movement, float deltaTime)
{
	switch (movement)
	{
	case CameraMovement::FORWARD:
		cameraPosition += cameraFront * cameraSpeed * deltaTime;
		break;
	case CameraMovement::BACKWARD:
		cameraPosition -= cameraFront * cameraSpeed * deltaTime;
		break;
	case CameraMovement::RIGHT:
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		break;
	case CameraMovement::LEFT:
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
		break;
	case CameraMovement::UP:
		cameraPosition.y += cameraSpeed * deltaTime;
		break;
	case CameraMovement::DOWN:
		cameraPosition.y -= cameraSpeed * deltaTime;
		break;
	default:
		break;
	}
}

void Camera::processMouseMove(float x, float y)
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
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));

	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp	= glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::processMouseScroll(float offset)
{
	if (fov >= 1.0f && fov <= 90.0f)
		fov -= offset;

	if (fov < 1.0f)
		fov = 1.0f;

	if (fov > 90.0f)
		fov = 90.0f;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}
