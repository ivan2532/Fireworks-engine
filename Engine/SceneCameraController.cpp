#include "SceneCameraController.hpp"
#include "glmIncludes.hpp"
#include "GLFW/glfw3.h"
#include "Window.hpp"
#include "Engine.hpp"
#include <iostream>

SceneCameraController::SceneCameraController(Window& window, float mSpeed, float lSens) noexcept
	:
	wnd(window),
	movementSpeed(mSpeed),
	lookSensitivity(lSens),
	dragX(0.0f),
	dragY(0.0f)
{
}

void SceneCameraController::OnMouseMove(float x, float y) noexcept
{
	static glm::vec3 euler;

	if (wnd.GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (!dragging)
		{
			dragging = true;
			dragX = x;
			dragY = y;
			euler = transform->GetEulerAngles();
		}
		else
		{
			transform->SetEulerAngles(
				glm::clamp(euler.x + (dragY - y) * lookSensitivity, -89.0f, 89.0f),
				glm::mod(euler.y + (x - dragX) * lookSensitivity, 360.0f),
				0.0f
			);
		}
	}
	else
		dragging = false;
}

std::unique_ptr<Component> SceneCameraController::Clone(GameObject* go) const noexcept
{
	auto result = std::make_unique<SceneCameraController>(*this);
	result->SetObject(go);
	result->Initialize();
	return std::move(result);
}

void SceneCameraController::Update() noexcept
{
	if (wnd.GetKey(GLFW_KEY_W))
	{
		transform->Translate(transform->GetForward() * movementSpeed * Engine::deltaTime);
	}
	if (wnd.GetKey(GLFW_KEY_A))
	{
		transform->Translate(-transform->GetRight() * movementSpeed * Engine::deltaTime);
	}
	if (wnd.GetKey(GLFW_KEY_S))
	{
		transform->Translate(-transform->GetForward() * movementSpeed * Engine::deltaTime);
	}
	if (wnd.GetKey(GLFW_KEY_D))
	{
		transform->Translate(transform->GetRight() * movementSpeed * Engine::deltaTime);
	}
}

void SceneCameraController::Initialize() noexcept
{
	transform = gameObject->GetComponent<Transform>().value();
}
