#pragma once
#include "Component.hpp"
#include "glmIncludes.hpp"
#include <memory>
#include <vector>

class Shader;

class Transform : public Component
{
public:
	Transform() noexcept;
	~Transform() noexcept;
	Transform(const Transform&) noexcept;
	Transform& operator=(const Transform&) noexcept;
	Transform(Transform&&) noexcept;
	Transform& operator=(Transform&&) noexcept;
public:
	void SetPosition(glm::vec3 value) noexcept;
	void SetPosition(float x, float y, float z) noexcept;
	glm::vec3 GetPosition() const noexcept;

	void SetEulerAngles(glm::vec3 value) noexcept;
	void SetEulerAngles(float x, float y, float z) noexcept;
	glm::vec3 GetEulerAngles() const noexcept;

	void SetScale(glm::vec3 value) noexcept;
	void SetScale(float x, float y, float z) noexcept;
	glm::vec3 GetScale() const noexcept;

	glm::mat4 GetTransformation() const noexcept;

	void Translate(glm::vec3 value) noexcept;
	void Rotate(glm::vec3 value) noexcept; // in degrees not radians
	void Scale(glm::vec3 value) noexcept;

	void AddShaderToUpdate(std::unique_ptr<Shader> shader) noexcept;
public:
	void Update() noexcept override;
private:
	void UpdateTransform() noexcept;
	void UpdateShaders() noexcept;
private:
	//Properties
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;
	glm::mat4 transform;
	std::unique_ptr<Transform> parent;
	//Shaders
	std::vector<std::unique_ptr<Shader>> shadersToUpdate;
};