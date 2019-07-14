#pragma once
#include "Window.hpp"
#include "GameObject.hpp"
#include "ImGuiManager.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "GameObject.hpp"
#include "ComponentIncludes.hpp"

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
	Window wnd;
	//User variables here
	GameObject cameraObject;
	ImGuiManager imGuiManager;
	Shader unlitTextureShader;
	GameObject ballObject;
public:
	static float deltaTime;
};

void CursorMoveCallback(GLFWwindow* wnd, double x, double y);