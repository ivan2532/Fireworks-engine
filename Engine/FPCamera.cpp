#include "FPCamera.hpp"
#include "Window.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

FPCamera::FPCamera(float movSpeed, float lookSens, glm::vec3 position, glm::vec3 rotation)
	:
	movementSpeed(movSpeed),
	lookSensitivity(lookSens),
	pos(position),
	rot(rotation)
{
	CalculateViewMatrix();
}

glm::mat4 FPCamera::GetViewMatrix() const noexcept
{
	return viewMatrix;
}

glm::vec3 FPCamera::GetPosition() const noexcept
{
	return pos;
}

glm::vec3 FPCamera::GetRotation() const noexcept
{
	return rot;
}

void FPCamera::KeyboardUpdate(Window& wnd, float dt) noexcept
{
	bool updateView = false;

	if (wnd.GetKey(GLFW_KEY_W))
	{
		pos += forward * movementSpeed * dt;
		updateView = true;
	}
	if (wnd.GetKey(GLFW_KEY_A))
	{
		pos -= right * movementSpeed * dt;
		updateView = true;
	}
	if (wnd.GetKey(GLFW_KEY_S))
	{
		pos -= forward * movementSpeed * dt;
		updateView = true;
	}
	if (wnd.GetKey(GLFW_KEY_D))
	{
		pos += right * movementSpeed * dt;
		updateView = true;
	}

	if (updateView)
		CalculateViewMatrix();
}

void FPCamera::MouseUpdate(float x, float y, float dt) noexcept
{
	if (firstMouse)
	{
		lastMouseX = x;
		lastMouseY = y;
		firstMouse = false;
	}

	float deltaX = (x - lastMouseX) * lookSensitivity;
	float deltaY = (lastMouseY - y) * lookSensitivity;

	lastMouseX = x;
	lastMouseY = y;

	rot.x = glm::clamp(rot.x + deltaY, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	rot.y = glm::mod(rot.y + deltaX, glm::two_pi<float>());

	CalculateViewMatrix();
}

void FPCamera::CalculateViewMatrix() noexcept
{
	forward.x = sin(rot.y) * cos(rot.x);
	forward.y = sin(rot.x);
	forward.z = -cos(rot.y) * cos(rot.x);
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	viewMatrix = glm::lookAt(pos, pos + forward, up);
}
