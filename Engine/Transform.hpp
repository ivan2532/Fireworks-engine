#pragma once
#include "Component.hpp"
#include "glmIncludes.hpp"
#include <memory>
#include <vector>

class GameObject;
class Shader;

class Transform : public Component
{
	friend class Camera;
public:
	Transform(GameObject* go) noexcept;
	~Transform() noexcept;
	Transform(const Transform&) noexcept;
	Transform& operator=(const Transform&) noexcept;
	Transform(Transform&&) noexcept;
	Transform& operator=(Transform&&) noexcept;
public:
	void SetPosition(float x, float y, float z) noexcept;
	void SetPosition(glm::vec3 value) noexcept;
	glm::vec3 GetPosition() const noexcept;

	void SetEulerAngles(float x, float y, float z) noexcept;
	void SetEulerAngles(glm::vec3 value) noexcept;
	glm::vec3 GetEulerAngles() const noexcept;

	void SetScale(float x, float y, float z) noexcept;
	void SetScale(glm::vec3 value) noexcept;
	glm::vec3 GetScale() const noexcept;

	glm::vec3 GetForward() const noexcept;
	glm::vec3 GetRight() const noexcept;
	glm::vec3 GetUp() const noexcept;

	glm::mat4 GetTransformation() const noexcept;

	void Translate(float x, float y, float z) noexcept;
	void Translate(glm::vec3 value) noexcept;
	void Rotate(float x, float y, float z) noexcept;
	void Rotate(glm::vec3 value) noexcept; // in degrees not radians
	void Scale(float x, float y, float z) noexcept;
	void Scale(glm::vec3 value) noexcept;

	void AddShaderToUpdate(std::unique_ptr<Shader> shader) noexcept;
public:
	void Update() noexcept override;
private:
	void UpdateTransform() noexcept;
	void UpdateShaders() noexcept;
private:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 transform;
	std::unique_ptr<Transform> parent;
	//Shaders
	std::vector<std::unique_ptr<Shader>> shadersToUpdate;

	bool updateAxes; //No need to calculate axes every frame
public:
	static constexpr glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
	static constexpr glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
	static constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
};