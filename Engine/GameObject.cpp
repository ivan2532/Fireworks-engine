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
	{
		components[i].reset(rhs.components[i].get());
		components[i]->SetObject(this);
	}
}

GameObject& GameObject::operator=(const GameObject& rhs) noexcept
{
	name = rhs.name;

	components.resize(rhs.components.size());
	for (unsigned i = 0; i < rhs.components.size(); i++)
	{
		components[i].reset(rhs.components[i].get());
		components[i]->SetObject(this);
	}

	return *this;
}

GameObject::GameObject(GameObject&& rhs) noexcept
	:
	name(std::move(rhs.name)),
	components(std::move(rhs.components))
{
	for (auto& c : components)
		c->SetObject(this);
}

GameObject& GameObject::operator=(GameObject&& rhs) noexcept
{
	name = std::move(rhs.name);
	components = std::move(rhs.components);

	for (auto& c : components)
		c->SetObject(this);

	return *this;
}

std::string GameObject::GetName() const noexcept
{
	return name;
}

void GameObject::SetName(const std::string& value) noexcept
{
	name = value;
}

void GameObject::Update() noexcept
{
	for (auto& component : components)
		component->Update();
}
