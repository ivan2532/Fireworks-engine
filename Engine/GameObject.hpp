#pragma once
#include <vector>
#include <memory>
#include <optional>

class Component;

class GameObject
{
public:
	GameObject() = default;
	void Update() noexcept;
	void AddComponent(std::unique_ptr<Component> component) noexcept;
	template<class T>
	std::optional<T*> GetComponent() const noexcept;
private:
	std::vector<std::unique_ptr<Component>> components;
};

template<class T>
std::optional<T*> GameObject::GetComponent() const noexcept
{
	for (auto& component : components)
		if (auto check = dynamic_cast<T*>(component.get()))
			return check;

	return {};
}
