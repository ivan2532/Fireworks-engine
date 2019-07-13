#pragma once
#include "Window.hpp"
#include "ImGuiManager.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "FPCamera.hpp"

class Game
{
public:
	Game() noexcept;
	~Game() noexcept;
	void Go() noexcept;
	//User functions here
	void MouseUpdate(double x, double y) noexcept;
private:
	void BeginFrame() noexcept;
	void Update() noexcept;
	void EndFrame(Window& wnd) noexcept;
	//User functions here
	void ProcessInput() noexcept;
	void RenderFrame() noexcept;
private:
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
	Window wnd;
	//User variables here
	ImGuiManager imGuiManager;
	FPCamera camera;
	Model materialBall;
	Model nanosuit;
	Shader unlitTextureShader;
};

void CursorMoveCallback(GLFWwindow* wnd, double x, double y);