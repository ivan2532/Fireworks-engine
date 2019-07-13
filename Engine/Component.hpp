#pragma once
#include "GameObject.hpp"

class Component
{
	friend class GameObject;
public:
	Component() = default;
	GameObject* GetObject() const noexcept;
	virtual void Update() noexcept = 0;
private:
	void SetObject(GameObject* go) noexcept;
protected:
	GameObject* gameObject;
};