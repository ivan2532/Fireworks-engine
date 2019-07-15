#pragma once
#include "Component.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>

class GameObject
{
	friend class Scene;
public:
	GameObject(const std::string& name) noexcept;
	~GameObject() = default;
	GameObject(const GameObject&) noexcept;
	GameObject& operator=(const GameObject&) noexcept;
	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;
public:
	std::string GetName() const noexcept;
	void Update() noexcept;
	void AddComponent(std::unique_ptr<Component> component) noexcept;
	template<class T>
	std::optional<T*> GetComponent() const noexcept;
private:
	std::string name;
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
