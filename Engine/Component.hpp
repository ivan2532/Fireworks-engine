#pragma once

class GameObject;

class Component
{
	friend class Scene;
public:
	Component(GameObject* go) noexcept;
public:
	GameObject* GetObject() const noexcept;
	virtual void Update() noexcept = 0;
	virtual void DrawInspector() noexcept;
private:
	virtual void OnMouseMove(float x, float y) noexcept;
	virtual void OnWindowResize(int width, int height) noexcept;
protected:
	GameObject* gameObject;
};