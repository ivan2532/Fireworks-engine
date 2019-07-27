#pragma once
#include "Component.hpp"
#include "ComponentIncludes.hpp"

class Window;

class FPCameraMovement : public Component
{
public:
	FPCameraMovement(Window& window, float mSpeed = 7.0f, float lSens = 0.2f) noexcept;
public:
	void DrawInspector(Editor& editor) noexcept override;
	void OnMouseMove(float x, float y) noexcept override;
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