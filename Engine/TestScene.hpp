#pragma once
#include "Scene.hpp"
#include "Shader.hpp"

class Window;

class TestScene : public Scene
{
public:
	TestScene(Window& wnd) noexcept;
	void Update() noexcept override;
	void MouseCallback(float x, float y) noexcept override;
private:
	Window& wnd;
	Shader textureShader;
};