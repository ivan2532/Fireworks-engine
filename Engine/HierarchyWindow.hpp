#pragma once
#include "EditorWindow.hpp"

class Engine;

class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
private:
	int nodeIndexCount = 0;
};