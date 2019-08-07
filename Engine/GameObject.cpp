#include "GameObject.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include "Scene.hpp"
#include <iostream>

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
		components[i] = std::move(rhs.components[i]->Clone(this));
		components[i]->SetObject(this);
	}

	auto transform = rhs.GetComponent<Transform>();

	if (transform)
	{
		for (auto& child : transform.value()->children)
			child->SetParent(transform.value(), false);
	}
}

GameObject& GameObject::operator=(const GameObject& rhs) noexcept
{
	deleteFlag = rhs.deleteFlag;
	name = rhs.name;

	components.resize(rhs.components.size());
	for (unsigned i = 0; i < rhs.components.size(); i++)
	{
		components[i] = std::move(rhs.components[i]->Clone(this));
		components[i]->SetObject(this);
	}

	auto transform = rhs.GetComponent<Transform>();

	if (transform)
	{
		for (auto& child : transform.value()->children)
			child->SetParent(transform.value(), false);
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
	deleteFlag = rhs.deleteFlag;
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

void GameObject::Delete(bool deleteChildren) noexcept
{
	auto transform = GetComponent<Transform>().value();

	if (deleteChildren)
	{
		for (unsigned i = 0; i < transform->GetChildCount(); i++)
			transform->GetChild(i)->GetObject()->Delete();
	}

	deleteFlag = true;

	if (transform->parent)
		transform->parent->CheckChildrenDelete();
}

GameObject& GameObject::AddToScene(Scene& scene, bool addChildren, Transform* parent) noexcept
{
	std::cout << "Adding game object \"" << name << "\"" << std::endl;

	auto newGameObject = scene.AddSceneObject(*this);

	auto transform = newGameObject.GetComponent<Transform>();

	if (parent != nullptr)
	{
		if (transform)
			transform.value()->SetParent(parent);
	}

	if (addChildren && transform)
	{
		for (auto& child : transform.value()->children)
			child->gameObject->AddToScene(scene);
	}

	return newGameObject;
}
