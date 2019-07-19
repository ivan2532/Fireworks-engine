#include "FPCameraMovement.hpp"
#include "glmIncludes.hpp"
#include "GLFW/glfw3.h"
#include "Window.hpp"
#include "Engine.hpp"
#include <iostream>

FPCameraMovement::FPCameraMovement(GameObject* go, Window& window, float mSpeed, float lSens) noexcept
	:
	Component(go),
	wnd(window),
	transform(gameObject->GetComponent<Transform>().value()),
	movementSpeed(mSpeed),
	lookSensitivity(lSens),
	lastMouseX(0.0f),
	lastMouseY(0.0f)
{
}

void FPCameraMovement::DrawInspector() noexcept
{
	if (ImGui::CollapsingHeader("FPCameraMovement"))
	{
		ImGui::Text("Movement speed");
		ImGui::SameLine();
		ImGui::InputFloat("##input_mov_speed", &movementSpeed);

		ImGui::Text("Look sensitivity");
		ImGui::SameLine();
		ImGui::InputFloat("##input_look_sens", &lookSensitivity);
	}
}

void FPCameraMovement::OnMouseMove(float x, float y) noexcept
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
