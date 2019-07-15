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
private:
	virtual void OnMouseMove(float x, float y) noexcept;
protected:
	GameObject* gameObject;
};