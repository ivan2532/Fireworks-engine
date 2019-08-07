#include "Camera.hpp"
#include "Shader.hpp"
#include "Engine.hpp"
#include "imguiIncludes.hpp"

Camera::Camera(Engine& rEngine, float f) noexcept
	:
	engine(rEngine),
	viewMatrix(1.0f),
	fov(f)
{
	projectionMatrix = glm::perspective(
		glm::radians(fov),
		engine.editor.GetEditorWindow<SceneViewWindow>().value()->GetSceneViewAspectRatio(),
		0.1f,
		1000.0f
	);
}

std::unique_ptr<Component> Camera::Clone(GameObject* go) const noexcept
{
	auto result = std::make_unique<Camera>(*this);
	result->SetObject(go);
	result->Initialize();
	return std::move(result);
}

void Camera::Update() noexcept
{
	UpdateMatrices();
	UpdateShaders();
}

void Camera::DrawInspector(Editor& editor) noexcept
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		int input = (int)fov;
		ImGui::Text("FOV: ");
		ImGui::SameLine();
		if (ImGui::SliderInt("##slider_fov", &input, 1, 179))
			fov = (float)input;
	}
}

float Camera::GetFOV() const noexcept
{
	return fov;
}

void Camera::SetFOV(float value) noexcept
{
	fov = value;
}

glm::mat4 Camera::GetViewMatrix() const noexcept
{
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const noexcept
{
	return projectionMatrix;
}

void Camera::Initialize() noexcept
{
	transform = gameObject->GetComponent<Transform>().value();
}

void Camera::UpdateMatrices() noexcept
{
	if (transform == nullptr)
		return;
	
	projectionMatrix = glm::perspective(
		glm::radians(fov),
		engine.editor.GetEditorWindow<SceneViewWindow>().value()->GetSceneViewAspectRatio(),
		0.1f,
		1000.0f
	);
	viewMatrix = glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetForward(), transform->GetUp());

	engine.SetCamera(this);
}

void Camera::UpdateShaders() noexcept
{
	if (transform == nullptr)
		return;

	for (auto& shader : transform->shadersToUpdate)
	{
		shader->SetVec3("viewPos", transform->GetPosition());
		shader->SetMat4x4("viewProj", projectionMatrix * viewMatrix);
	}
}
