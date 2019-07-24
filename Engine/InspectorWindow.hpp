#pragma once
#include "EditorWindow.hpp"

class Engine;

class InspectorWindow : public EditorWindow
{
public:
	InspectorWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
};