#include "Game.hpp"
#include "OpenGLIncludes.hpp"
#include "imguiIncludes.hpp"
#include "TestScene.hpp"

float Game::deltaTime = 0.0f;

Game::Game() noexcept
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

Game::~Game() noexcept
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
}

void Game::Go() noexcept
{
	while (!wnd.ShouldClose())
	{
		BeginFrame();
		Update();
		EndFrame(wnd);
	}
}

void Game::BeginFrame() noexcept
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

void Game::Update() noexcept
{
	ProcessInput();
	activeScene->Update();
}

void Game::EndFrame(Window& wnd) noexcept
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	wnd.EndFrame();
}

void Game::ProcessInput() noexcept
{
	if (wnd.GetKey(GLFW_KEY_ESCAPE))
		wnd.Close();
}

void Game::MouseUpdate(double x, double y) noexcept
{
	activeScene->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
}

void CursorMoveCallback(GLFWwindow* wnd, double x, double y)
{
	reinterpret_cast<Game*>(glfwGetWindowUserPointer(wnd))->MouseUpdate(x, y);
}
