#pragma once
#include "Component.hpp"
#include "ComponentIncludes.hpp"
#include "glmIncludes.hpp"
#include "GameObject.hpp"

class Camera : public Component
{
public:
	Camera(GameObject* go, float fov = 70.0f) noexcept;
public:
	void Update() noexcept override;
public:
	void DrawInspector() noexcept override;
	float GetFOV() const noexcept;
	void SetFOV(float value) noexcept;
private:
	void UpdateViewMatrix() noexcept;
	void UpdateShaders() noexcept;
private:
	Transform* transform;
	glm::mat4 viewMatrix;
	float fov; //In degrees!
};