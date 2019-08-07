#pragma once
#include "ComponentUndoable.hpp"
#include <memory>

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
	virtual std::unique_ptr<Component> Clone(GameObject* go) const noexcept = 0;
	virtual void Update() noexcept = 0;
	virtual void DrawInspector(Editor& editor) noexcept {}
protected:
	virtual void Initialize() noexcept {}
	virtual void OnMouseMove(float x, float y) noexcept {}
	virtual void OnWindowResize(int width, int height) noexcept {}
protected:
	template <class T>
	void PushUndoable(Editor& editor, void* target, void* startState, void* endState) noexcept;
public:
	GameObject* gameObject = nullptr;
protected:
	void* startState = nullptr;
	void* endState = nullptr;
};
