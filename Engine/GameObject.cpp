#include "GameObject.hpp"

GameObject::GameObject(const std::string& n) noexcept
	:
	name(n)
{
}

std::string GameObject::GetName() const noexcept
{
	return name;
}

void GameObject::Update() noexcept
{
	for (auto& component : components)
		component->Update();
}

void GameObject::AddComponent(std::unique_ptr<Component> component) noexcept
{
	components.push_back(std::move(component));
}
