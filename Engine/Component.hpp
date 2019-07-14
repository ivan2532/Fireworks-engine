#pragma once
#include "GameObject.hpp"

class Component
{
public:
	Component(GameObject* go) noexcept;
	GameObject* GetObject() const noexcept;
	virtual void Update() noexcept = 0;
protected:
	GameObject* gameObject;
};