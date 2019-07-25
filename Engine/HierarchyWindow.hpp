#pragma once
#include "EditorWindow.hpp"
#include <vector>
#include <memory>

class Engine;
class GameObject;

class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
	int GetSelectedHierarchy() const noexcept;
	void SetSelectedHierarchy(int value) noexcept;
private:
	int nodeIndexCount = 0;
	int selectedHierarchy = -1;
};