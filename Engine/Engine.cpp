#include "Engine.hpp"
#include "OpenGLIncludes.hpp"
#include "imguiIncludes.hpp"
#include "TestScene.hpp"
#include <iostream>

float Engine::deltaTime = 0.0f;

Engine::Engine() noexcept
	:
	wnd(1920u, 1080u, "Fireworks engine", false, true, false, true),
	editor(*this),
	activeScene(std::make_unique<TestScene>(*this, wnd))
{
	//wnd.MakeFramebuffer(800u, 600u);

	SetCallbacks();

	ImGui_ImplGlfw_InitForOpenGL(wnd.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

Engine::~Engine() noexcept
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
}

void Engine::Go() noexcept
{
	while (!wnd.ShouldClose())
	{
		BeginFrame();
		Update();
		EndFrame(wnd);
	}
}

void Engine::SetCallbacks() noexcept
{
	glfwSetWindowUserPointer(wnd.GetWindow(), (void*)this);

	glfwSetCursorPosCallback(
		wnd.GetWindow(),
		CursorMoveCallback
	);

	glfwSetFramebufferSizeCallback(
		wnd.GetWindow(),
		FramebufferSizeCallback
	);
}

void Engine::ClearBuffers() noexcept
{
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::BeginFrame() noexcept
{
	float curTime = (float)glfwGetTime();
	deltaTime = curTime - lastFrame;
	lastFrame = curTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();


	wnd.BindFramebuffer(); //Unbound in editor draw scene view
	ClearBuffers();
}

void Engine::Update() noexcept
{
	ProcessInput();
	activeScene->Update();
	editor.DrawGUI();
}

void Engine::EndFrame(Window& wnd) noexcept
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	wnd.EndFrame();
}

void Engine::ProcessInput() noexcept
{
	if (wnd.GetKey(GLFW_KEY_ESCAPE, false))
		wnd.Close();
}

void Engine::SetCamera(Camera* camera) noexcept
{
	activeCamera = camera;
}

Window* Engine::GetWindow() noexcept
{
	return &wnd;
}

void Engine::OnMouseMove(double x, double y) noexcept
{
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	activeScene->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
}

void Engine::OnWindowResize(int width, int height) noexcept
{
	wnd.SetWidth(width);
	wnd.SetHeight(height);
	activeScene->OnWindowResize(width, height);
}

void CursorMoveCallback(GLFWwindow* wnd, double x, double y)
{
	reinterpret_cast<Engine*>(glfwGetWindowUserPointer(wnd))->OnMouseMove(x, y);
}

void FramebufferSizeCallback(GLFWwindow* wnd, int width, int height)
{
	reinterpret_cast<Engine*>(glfwGetWindowUserPointer(wnd))->OnWindowResize(width, height);
	glViewport(0, 0, width, height);
}
