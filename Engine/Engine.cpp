#include "Engine.hpp"
#include "OpenGLIncludes.hpp"
#include "imguiIncludes.hpp"
#include "TestScene.hpp"

float Engine::deltaTime = 0.0f;

Engine::Engine() noexcept
	:
	wnd(1920u, 1080u, "Fireworks engine", false, true, false, true),
	editor(*this),
	activeScene(std::make_unique<TestScene>(wnd))
{
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

void Engine::BeginFrame() noexcept
{
	glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float curTime = (float)glfwGetTime();
	deltaTime = curTime - lastFrame;
	lastFrame = curTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Engine::Update() noexcept
{
	editor.DrawGUI();
	ProcessInput();
	activeScene->Update();
}

void Engine::EndFrame(Window& wnd) noexcept
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	wnd.EndFrame();
}

void Engine::ProcessInput() noexcept
{
	if (wnd.GetKey(GLFW_KEY_ESCAPE))
		wnd.Close();
}

void Engine::OnMouseMove(double x, double y) noexcept
{
	activeScene->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
}

void Engine::OnWindowResize(int width, int height) noexcept
{
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
