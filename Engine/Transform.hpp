#pragma once
#include "Component.hpp"
#include "glmIncludes.hpp"
#include <memory>
#include <vector>

class GameObject;
class Shader;
class Editor;

class Transform : public Component
{
	friend class Camera;
public:
	Transform() noexcept;
	Transform(const Transform&) noexcept;
	Transform& operator=(const Transform&) noexcept;
	Transform(Transform&&) noexcept;
	Transform& operator=(Transform&&) noexcept;
public:
	void SetPosition(float x, float y, float z) noexcept;
	void SetPosition(const glm::vec3& value) noexcept;
	glm::vec3 GetPosition() const noexcept;

	void SetEulerAngles(float x, float y, float z) noexcept;
	void SetEulerAngles(const glm::vec3& value) noexcept;
	glm::vec3 GetEulerAngles() const noexcept;

	void SetScale(float x, float y, float z) noexcept;
	void SetScale(const glm::vec3& value) noexcept;
	glm::vec3 GetScale() const noexcept;

	glm::vec3 GetForward() const noexcept;
	glm::vec3 GetRight() const noexcept;
	glm::vec3 GetUp() const noexcept;

	void SetTransformation(glm::mat4 transformation, bool subtractParentTransform) noexcept;
	glm::mat4 GetTransformation() const noexcept;

	void Translate(float x, float y, float z) noexcept;
	void Translate(const glm::vec3& value) noexcept;
	void Rotate(float x, float y, float z) noexcept;
	void Rotate(const glm::vec3& value) noexcept; // in degrees not radians
	void Scale(float x, float y, float z) noexcept;
	void Scale(const glm::vec3& value) noexcept;

	Transform* GetChild(int i) const noexcept;

	Transform* GetParent() const noexcept;
	void SetParent(Transform* parent) noexcept;

	void AddShaderToUpdate(Shader* shader) noexcept;
public:
	void DrawHierarchy(Editor& editor, int& nodeIndexCount) const noexcept;
public:
	void Update() noexcept override;
	void DrawInspector() noexcept override;
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

	glm::mat4 accumulatedTransform = glm::mat4(1.0f);
	Transform* parent;
	std::vector<Transform*> children;
	//Shaders
	std::vector<Shader*> shadersToUpdate;
	bool updateAxes; //No need to calculate axes every frame
public:
	static constexpr glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
	static constexpr glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
	static constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
};