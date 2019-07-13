#pragma once
#include "Component.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <memory>
#include <vector>

class Shader;

class Transform : public Component
{
public:
	Transform() noexcept;
	Transform(const Transform&) = default; //FIX
	Transform& operator=(const Transform&) = default; //FIX
public:
	void SetPosition(glm::vec3 value) noexcept;
	glm::vec3 GetPosition() const noexcept;

	void SetEulerAngles(glm::vec3 value) noexcept;
	glm::vec3 GetEulerAngles() const noexcept;

	void SetScale(glm::vec3 value) noexcept;
	glm::vec3 GetScale() const noexcept;

	glm::mat4 GetTransformation() const noexcept;

	void Translate(glm::vec3 value) noexcept;
	void Rotate(glm::vec3 value) noexcept; // in degrees not radians

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