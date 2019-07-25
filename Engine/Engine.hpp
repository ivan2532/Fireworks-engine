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
	friend Window;
	friend Camera;
	friend class HierarchyWindow;
public:
	Engine() noexcept;
	~Engine() noexcept;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine&& operator=(Engine&&) = delete;
	void Go() noexcept;
public:
	void OnMouseMove(double x, double y) noexcept;
	void OnWindowResize(int width, int height) noexcept;
	void SetCamera(Camera* camera) noexcept;
	Window* GetWindow() noexcept;
	Camera* GetActiveCamera() const noexcept;
private:
	void SetCallbacks() noexcept;
	void ClearBuffers() noexcept;
	void BeginFrame() noexcept;
	void Update() noexcept;
	void EndFrame(Window& wnd) noexcept;
	void ProcessInput() noexcept;
	//User functions here
private:
	float lastFrame = 0.0f;
	Window wnd;
	Editor editor;
	ImGuiManager imGuiManager;
	Camera* activeCamera;
	std::unique_ptr<Scene> activeScene;
	//User variables here
public:
	static float deltaTime;
};

void CursorMoveCallback(GLFWwindow* wnd, double x, double y);
void FramebufferSizeCallback(GLFWwindow* wnd, int width, int height);