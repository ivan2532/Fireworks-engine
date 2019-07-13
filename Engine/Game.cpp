#include "Game.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

Game::Game() noexcept //Window initialized first so glfwInit() is called in the window
	//:
	//wnd(800u, 600u, "Test", false)
	:
	wnd(false),
	camera(20.0f, 0.005f, glm::vec3(0.0f, 0.0f, 50.0f), glm::zero<glm::vec3>()),
	materialBall("Material_ball/material_ball.obj"),
	nanosuit("Nanosuit/nanosuit.obj"),
	unlitTextureShader("UnlitTextureVS.glsl", "UnlitTextureFS.glsl")
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
	RenderFrame();
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

	camera.KeyboardUpdate(wnd, deltaTime);
}

void Game::MouseUpdate(double x, double y) noexcept
{
	camera.MouseUpdate((float)x, (float)y, deltaTime);
}

void Game::RenderFrame() noexcept
{
	glm::mat4 viewProj =
		glm::perspective(glm::radians(80.0f), (float)wnd.GetWidth() / (float)wnd.GetHeight(), 0.1f, 1000.0f) *
		camera.GetViewMatrix();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	unlitTextureShader.SetMat4x4("viewProj", viewProj);
	unlitTextureShader.SetMat4x4("model", model);

	//TEST
	unlitTextureShader.SetVec3("viewPos", camera.GetPosition());
	unlitTextureShader.SetVec3("lightPos", glm::vec3(sin(glfwGetTime()) * 5.0f, sin(glfwGetTime()) * 2.5f, 10.0f));
	unlitTextureShader.SetVec4("pointLight.lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	unlitTextureShader.SetFloat("pointLight.ambientStrength", 0.1f);
	unlitTextureShader.SetFloat("pointLight.shininess", 32.0f);
	unlitTextureShader.SetFloat("pointLight.specularStrength", 0.7f);
	unlitTextureShader.SetFloat("pointLight.intensity", 2.0f);
	unlitTextureShader.SetFloat("pointLight.attenuation_const", 1.0f);
	unlitTextureShader.SetFloat("pointLight.attenuation_linear", 0.045f);
	unlitTextureShader.SetFloat("pointLight.attenuation_quadratic", 0.0075f);
	//TEST

	materialBall.Draw(unlitTextureShader);

	glm::mat4 model2(1.0f);
	model2 = glm::translate(model2, glm::vec3(20.0f, 0.0f, 0.0f));
	//model = glm::rotate(model2, (float)glfwGetTime() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	model2 = glm::scale(model2, glm::vec3(2.0f, 2.0f, 2.0f));
	unlitTextureShader.SetMat4x4("viewProj", viewProj);
	unlitTextureShader.SetMat4x4("model", model2);

	nanosuit.Draw(unlitTextureShader);

	if (ImGui::Begin("Hello world window!"))
	{
		ImGui::Text("Hello world!");
	}
	ImGui::End();
}

void CursorMoveCallback(GLFWwindow* wnd, double x, double y)
{
	reinterpret_cast<Game*>(glfwGetWindowUserPointer(wnd))->MouseUpdate(x, y);
}
