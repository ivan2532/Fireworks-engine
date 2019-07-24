#pragma once
#include "EditorWindow.hpp"
#include "imguiIncludes.hpp"

class Engine;

class SceneViewWindow : public EditorWindow
{
public:
	SceneViewWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
};