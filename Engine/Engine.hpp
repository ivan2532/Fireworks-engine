#pragma once
#include "Window.hpp"
#include "GameObject.hpp"
#include "ImGuiManager.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "GameObject.hpp"
#include "ComponentIncludes.hpp"
#include "Scene.hpp"
#include "Editor.hpp"

class Engine
{
	friend Editor;
public:
	Engine() noexcept;
	~Engine() noexcept;
	void Go() noexcept;
public:
	void OnMouseMove(double x, double y) noexcept;
	void OnWindowResize(int width, int height) noexcept;
private:
	void SetCallbacks() noexcept;
	void BeginFrame() noexcept;
	void Update() noexcept;
	void EndFrame(Window& wnd) noexcept;
	//User functions here
	void ProcessInput() noexcept;
private:
	float lastFrame = 0.0f;
	Window wnd;
	Editor editor;
	//User variables here
	ImGuiManager imGuiManager;
	std::unique_ptr<Scene> activeScene;
public:
	static float deltaTime;
};

void CursorMoveCallback(GLFWwindow* wnd, double x, double y);
void FramebufferSizeCallback(GLFWwindow* wnd, int width, int height);