#include "Camera.hpp"
#include "Shader.hpp"
#include "imguiIncludes.hpp"

Camera::Camera(GameObject* go, float sWidth, float sHeight, float f) noexcept
	:
	Component(go),
	transform(gameObject->GetComponent<Transform>().value()),
	viewMatrix(1.0f),
	screenWidth(sWidth),
	screenHeight(sHeight),
	fov(f)
{
}

void Camera::Update() noexcept
{
	UpdateViewMatrix();
	UpdateShaders();
}

void Camera::DrawInspector() noexcept
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		int input = fov;
		ImGui::Text("FOV: ");
		ImGui::SameLine();
		if (ImGui::SliderInt("##slider_fov", &input, 0.1f, 179.9f))
			fov = (float)input;
	}
}

void Camera::OnWindowResize(int width, int height) noexcept
{
	screenWidth = width;
	screenHeight = height;
}

float Camera::GetFOV() const noexcept
{
	return fov;
}

void Camera::SetFOV(float value) noexcept
{
	fov = value;
}

void Camera::UpdateViewMatrix() noexcept
{
	viewMatrix = glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
}

void Camera::UpdateShaders() noexcept
{
	for (auto& shader : transform->shadersToUpdate)
	{
		shader->SetVec3("viewPos", transform->GetPosition());
		shader->SetMat4x4("viewProj", glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 1000.0f) * viewMatrix);
	}
}
