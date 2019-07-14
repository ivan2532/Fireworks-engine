#include "Camera.hpp"
#include "Shader.hpp"

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
