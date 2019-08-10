#pragma once
#include "EditorWindow.hpp"
#include "GameObject.hpp"

class Engine;

class InspectorWindow : public EditorWindow
{
	friend class HierarchyWindow;
public:
	InspectorWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
public:
	GameObject* GetSelectedObject() const noexcept;
	void SetSelectedObject(GameObject* value) noexcept;
private:
	GameObject* selectedObject = nullptr;
};