#include "Game.hpp"
#include "OpenGLIncludes.hpp"
#include "imguiIncludes.hpp"

float Game::deltaTime = 0.0f;

Game::Game() noexcept
	//:
	//wnd(800u, 600u, "Test", false)
	:
	wnd(true),
	unlitTextureShader("UnlitTextureVS.glsl", "UnlitTextureFS.glsl")
{
	glfwSetWindowUserPointer(wnd.GetWindow(), (void*)this);

	glfwSetCursorPosCallback(
		wnd.GetWindow(),
		CursorMoveCallback
	);

	ImGui_ImplGlfw_InitForOpenGL(wnd.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ballObject.AddComponent(std::make_unique<Transform>(&ballObject));
	ballObject.GetComponent<Transform>().value()->AddShaderToUpdate(std::make_unique<Shader>(unlitTextureShader));
	ballObject.AddComponent(std::make_unique<ModelRenderer>(&ballObject, "Material_ball/material_ball.obj", unlitTextureShader));

	cameraObject.AddComponent(std::make_unique<Transform>(&cameraObject));
	cameraObject.GetComponent<Transform>().value()->AddShaderToUpdate(std::make_unique<Shader>(unlitTextureShader));
	cameraObject.GetComponent<Transform>().value()->Translate(0.0f, 10.0f, 10.0f);
	cameraObject.AddComponent(std::make_unique<Camera>(&cameraObject, static_cast<float>(wnd.GetWidth()), static_cast<float>(wnd.GetHeight())));
	cameraObject.AddComponent(std::make_unique<FPCameraMovement>(&cameraObject, wnd, 7.0f, 0.5f));

	auto transComponent = ballObject.GetComponent<Transform>();

	if (transComponent.has_value())
	{
		transComponent.value()->Translate(0.0f, 0.0f, 0.0f);
	}
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
	RenderFrame();

	ballObject.Update();
	cameraObject.Update();
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
	cameraObject.GetComponent<FPCameraMovement>().value()->MouseCallback(static_cast<float>(x), static_cast<float>(y));
}

void Game::RenderFrame() noexcept
{
	//TEST
	unlitTextureShader.SetVec3("lightPos", glm::vec3(sin(glfwGetTime()) * 5.0f, sin(glfwGetTime()) * 2.5f, 8.0f));
	unlitTextureShader.SetVec4("pointLight.lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	unlitTextureShader.SetFloat("pointLight.ambientStrength", 0.1f);
	unlitTextureShader.SetFloat("pointLight.shininess", 32.0f);
	unlitTextureShader.SetFloat("pointLight.specularStrength", 0.7f);
	unlitTextureShader.SetFloat("pointLight.intensity", 2.0f);
	unlitTextureShader.SetFloat("pointLight.attenuation_const", 1.0f);
	unlitTextureShader.SetFloat("pointLight.attenuation_linear", 0.045f);
	unlitTextureShader.SetFloat("pointLight.attenuation_quadratic", 0.0075f);
	//TEST
}

void CursorMoveCallback(GLFWwindow* wnd, double x, double y)
{
	reinterpret_cast<Game*>(glfwGetWindowUserPointer(wnd))->MouseUpdate(x, y);
}
