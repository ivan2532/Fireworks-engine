#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Window;

class FPCamera
{
public:
	FPCamera(float movSpeed, float lookSens, glm::vec3 position = glm::zero<glm::vec3>(), glm::vec3 rotation = glm::zero<glm::vec3>());
public:
	glm::mat4 GetViewMatrix() const noexcept;
	glm::vec3 GetPosition() const noexcept;
	glm::vec3 GetRotation() const noexcept;
	void KeyboardUpdate(Window& wnd, float dt) noexcept;
	void MouseUpdate(float x, float y, float dt) noexcept;
private:
	void CalculateViewMatrix() noexcept;
private:
	glm::vec3 pos;
	glm::vec3 rot;
	float movementSpeed;
	float lookSensitivity;

	//Axes
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	glm::mat4 viewMatrix;
private:
	bool firstMouse = true;
	float lastMouseX, lastMouseY;
public:
	static constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
};