#pragma once
#include "Component.hpp"
#include "ComponentIncludes.hpp"
#include "glmIncludes.hpp"
#include "GameObject.hpp"

class Camera : public Component
{
public:
	Camera(GameObject* go, float screenWidth, float screenHeight, float fov = 70.0f) noexcept;
public:
	void Update() noexcept override;
public:
	void OnWindowResize(int width, int height) noexcept override;
private:
	void UpdateViewMatrix() noexcept;
	void UpdateShaders() noexcept;
private:
	Transform* transform;
	glm::mat4 viewMatrix;
	float screenWidth;
	float screenHeight;
	float fov; //In degrees!
};