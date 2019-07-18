#pragma once
#include "imguiIncludes.hpp"

class Engine;

class Editor
{
public:
	Editor(Engine& engine) noexcept;
	void DrawGUI() noexcept;
	void DrawHierarchyUI() noexcept;
private:
	Engine& engine;
};