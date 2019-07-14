#pragma once
#include "Component.hpp"
#include "ComponentIncludes.hpp"

class Window;

class FPCameraMovement : public Component
{
public:
	FPCameraMovement(GameObject* go, Window& window, float mSpeed = 1.0f, float lSens = 1.0f) noexcept;
public:
	void MouseCallback(float x, float y) noexcept;
public:
	void Update() noexcept override;
private:
	Window& wnd;
	Transform* transform;
	float movementSpeed;
	float lookSensitivity;
private: //Input helpers
	bool firstMouse = true;
	float lastMouseX;
	float lastMouseY;
};