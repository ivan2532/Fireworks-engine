#pragma once
#include "EditorWindow.hpp"
#include "GameObject.hpp"

class Engine;

class InspectorWindow : public EditorWindow
{
public:
	InspectorWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
	GameObject* GetSelectedObject() const noexcept;
	void SetSelectedObject(GameObject* value) noexcept;
private:
	GameObject* selectedObject = nullptr;
};