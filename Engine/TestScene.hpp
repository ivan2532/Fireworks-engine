#pragma once
#include "Scene.hpp"
#include "Shader.hpp"
#include "Model.hpp"

class Window;

class TestScene : public Scene
{
public:
	TestScene(Engine& rEngine, Window& wnd) noexcept;
	void Update() noexcept override;
private:
	Window& wnd;
	Model testModel;

	glm::vec3 lightPositions[4] = {
		   glm::vec3(-10.0f,  10.0f, 10.0f),
		   glm::vec3(10.0f,  10.0f, 10.0f),
		   glm::vec3(-10.0f, -10.0f, 10.0f),
		   glm::vec3(10.0f, -10.0f, 10.0f),
	};

	glm::vec3 lightColors[4] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
};