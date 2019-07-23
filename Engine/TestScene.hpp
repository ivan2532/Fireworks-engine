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
	Shader textureShader;
	Model testModel;
};