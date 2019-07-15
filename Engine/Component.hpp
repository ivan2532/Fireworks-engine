#pragma once

class GameObject;

class Component
{
public:
	Component(GameObject* go) noexcept;
public:
	GameObject* GetObject() const noexcept;
	virtual void Update() noexcept = 0;
protected:
	GameObject* gameObject;
};