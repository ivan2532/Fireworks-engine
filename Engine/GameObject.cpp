#include "GameObject.hpp"

GameObject::GameObject(const std::string& n) noexcept
	:
	name(n)
{
}

GameObject::GameObject(const GameObject& rhs) noexcept
	:
	name(rhs.name)
{
	components.resize(rhs.components.size());
	for (unsigned i = 0; i < rhs.components.size(); i++)
		components[i].reset(rhs.components[i].get());
}

GameObject& GameObject::operator=(const GameObject& rhs) noexcept
{
	name = rhs.name;

	components.resize(rhs.components.size());
	for (unsigned i = 0; i < rhs.components.size(); i++)
		components[i].reset(rhs.components[i].get());

	return *this;
}

GameObject::GameObject(GameObject&& rhs) noexcept
	:
	name(std::move(rhs.name)),
	components(std::move(rhs.components))
{
}

GameObject& GameObject::operator=(GameObject&& rhs) noexcept
{
	name = std::move(rhs.name);
	components = std::move(rhs.components);

	return *this;
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
