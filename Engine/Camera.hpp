#pragma once
#include "Component.hpp"
#include "ComponentIncludes.hpp"
#include "glmIncludes.hpp"
#include "GameObject.hpp"

class Camera : public Component
{
public:
	Camera(GameObject* go) noexcept;
public:
	void Update() noexcept override;
private:
	void UpdateViewMatrix() noexcept;
private:
	Transform* transform;
	glm::mat4 viewMatrix;
};