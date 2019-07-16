#include "Engine.hpp"
#include "OpenGLIncludes.hpp"
#include "imguiIncludes.hpp"
#include "TestScene.hpp"

float Engine::deltaTime = 0.0f;

Engine::Engine() noexcept
	//:
	//wnd(800u, 600u, "Test", false)
	:
	wnd(true),
	activeScene(std::make_unique<TestScene>(wnd))
{
	glfwSetWindowUserPointer(wnd.GetWindow(), (void*)this);

	glfwSetCursorPosCallback(
		wnd.GetWindow(),
		CursorMoveCallback
	);

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

void Engine::BeginFrame() noexcept
{
	glClearColor(0.0f, 0.137f, 0.219f, 1.0f);
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

void Engine::MouseUpdate(double x, double y) noexcept
{
	activeScene->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
}

void CursorMoveCallback(GLFWwindow* wnd, double x, double y)
{
	reinterpret_cast<Engine*>(glfwGetWindowUserPointer(wnd))->MouseUpdate(x, y);
}
