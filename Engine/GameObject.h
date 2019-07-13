#pragma once
#include <vector>
#include <memory>

class Component;

class GameObject
{
public:
	GameObject() = default;
	void Update() noexcept;
private:
	std::vector<std::unique_ptr<Component>> components;
};