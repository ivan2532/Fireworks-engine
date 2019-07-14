#include "Camera.hpp"

Camera::Camera(GameObject* go) noexcept
	:
	Component(go),
	transform(gameObject->GetComponent<Transform>().value()),
	viewMatrix(1.0f)
{
}

void Camera::Update() noexcept
{
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() noexcept
{
	viewMatrix = glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());
}
