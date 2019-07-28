#pragma once
#include "ComponentUndoable.hpp"
//#include "Editor.hpp"

class GameObject;
class Editor;

class Component
{
	friend class Scene;
	friend class GameObject;
public:
	Component() = default;
public:
	GameObject* GetObject() const noexcept;
	void SetObject(GameObject* go) noexcept;
	virtual void Update() noexcept = 0;
	virtual void DrawInspector(Editor& editor) noexcept {}
private:
	virtual void Initialize() noexcept {}
	virtual void OnMouseMove(float x, float y) noexcept {}
	virtual void OnWindowResize(int width, int height) noexcept {}
protected:
	template <class T>
	void PushUndoable(Editor& editor, void* target, void* startState, void* endState) noexcept;
protected:
	GameObject* gameObject = nullptr;
	void* startState = nullptr;
	void* endState = nullptr;
};
