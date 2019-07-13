#pragma once
#include <vector>
#include <memory>

class Component;

class GameObject
{
public:
	GameObject() = default;
	void Update() noexcept;
	void AddComponent(std::unique_ptr<Component> component) noexcept;
private:
	std::vector<std::unique_ptr<Component>> components;
};