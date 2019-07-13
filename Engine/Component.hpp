#pragma once

class Component
{
public:
	Component() = default;
	virtual void Update() noexcept = 0;
};