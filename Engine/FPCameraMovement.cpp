#include "FPCameraMovement.hpp"
#include "glmIncludes.hpp"
#include "GLFW/glfw3.h"
#include "Window.hpp"
#include "Game.hpp"
#include <iostream>

FPCameraMovement::FPCameraMovement(GameObject* go, Window& window, float mSpeed, float lSens) noexcept
	:
	Component(go),
	wnd(window),
	transform(gameObject->GetComponent<Transform>().value()),
	movementSpeed(mSpeed),
	lookSensitivity(lSens)
{
}

void FPCameraMovement::MouseCallback(float x, float y) noexcept
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

	auto eulerAngles = transform->GetEulerAngles();
	transform->SetEulerAngles(
		glm::clamp(eulerAngles.x + deltaY, -89.0f, 89.0f),
		glm::mod(eulerAngles.y + deltaX, 360.0f),
		0.0f
	);
}

void FPCameraMovement::Update() noexcept
{
	if (wnd.GetKey(GLFW_KEY_W))
	{
		transform->Translate(transform->GetForward() * movementSpeed * Game::deltaTime);
	}
	if (wnd.GetKey(GLFW_KEY_A))
	{
		transform->Translate(-transform->GetRight() * movementSpeed * Game::deltaTime);
	}
	if (wnd.GetKey(GLFW_KEY_S))
	{
		transform->Translate(-transform->GetForward() * movementSpeed * Game::deltaTime);
	}
	if (wnd.GetKey(GLFW_KEY_D))
	{
		transform->Translate(transform->GetRight() * movementSpeed * Game::deltaTime);
	}
}
