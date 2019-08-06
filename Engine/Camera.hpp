#pragma once
#include "Component.hpp"
#include "ComponentIncludes.hpp"
#include "glmIncludes.hpp"
#include "GameObject.hpp"

class Engine;

class Camera : public Component
{
public:
	Camera(Engine& rEngine, float fov = 70.0f) noexcept;
public:
	std::unique_ptr<Component> Clone() const noexcept override;
	void Update() noexcept override;
public:
	void DrawInspector(Editor& editor) noexcept override;
public:
	float GetFOV() const noexcept;
	void SetFOV(float value) noexcept;
	glm::mat4 GetViewMatrix() const noexcept;
	glm::mat4 GetProjectionMatrix() const noexcept;
private:
	void Initialize() noexcept override;
	void UpdateMatrices() noexcept;
	void UpdateShaders() noexcept;
private:
	Engine& engine;
	Transform* transform = nullptr;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	float fov; //In degrees!
};