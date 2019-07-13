#include "GameObject.hpp"
#include "Component.hpp"

void GameObject::Update() noexcept
{
	for (auto& component : components)
		component->Update();
}

void GameObject::AddComponent(std::unique_ptr<Component> component) noexcept
{
	component->SetObject(this);
	components.push_back(std::move(component));
}
