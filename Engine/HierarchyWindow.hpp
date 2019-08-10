#pragma once
#include "EditorWindow.hpp"
#include <memory>

class Engine;
class GameObject;
class InspectorWindow;

class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
	void ProcessInput() noexcept override;
public:
	int GetSelectedHierarchy() const noexcept;
	void SetSelectedHierarchy(int value) noexcept;
private:
	bool windowFocused = false;
	int nodeIndexCount = 0;
	int selectedHierarchy = -1;
	InspectorWindow* inspectorWindow;
};