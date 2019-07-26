#pragma once
#include "Component.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <utility>

class GameObject
{
	friend class Scene;
	friend class Editor;
	friend class InspectorWindow;
public:
	GameObject(const std::string& name) noexcept;
	~GameObject() = default;
	GameObject(const GameObject&) noexcept;
	GameObject& operator=(const GameObject&) noexcept;
	GameObject(GameObject&&) noexcept;
	GameObject& operator=(GameObject&&) noexcept;
public:
	std::string GetName() const noexcept;
	void SetName(const std::string&) noexcept;
	void Update() noexcept;

	template<class T, typename... Args>
	void AddComponent(Args&&... args) noexcept;

	template<class T>
	std::optional<T*> GetComponent() const noexcept;
private:
	std::string name;
	std::vector<std::unique_ptr<Component>> components;
};

template<typename T, class ...Args>
void GameObject::AddComponent(Args&&... args) noexcept
{
	components.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
	components.back()->SetObject(this);
	components.back()->Initialize();
}

template<class T>
std::optional<T*> GameObject::GetComponent() const noexcept
{
	for (auto& component : components)
		if (auto check = dynamic_cast<T*>(component.get()))
			return check;

	return {};
}
