#pragma once
#include "imguiIncludes.hpp"
#include "GameObject.hpp"

class Engine;

class Editor
{
public:
	Editor(Engine& engine) noexcept;
	void DrawGUI() noexcept;
	//Hierarchy
	void DrawHierarchyUI() noexcept;
	int GetSelectedHierarchy() const noexcept;
	void SetSelectedHierarchy(int value) noexcept;
	GameObject* GetSelectedObject() const noexcept;
	void SetSelectedObject(GameObject* value) noexcept;
	//Inspector
	void DrawInspectorUI() noexcept;
private:
	Engine& engine;
	//Hierarchy variables
	GameObject* selectedObject;
	int nodeIndexCount;
	int selectedHierarchy;
};