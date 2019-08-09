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

	auto rhsIt = rhs.components.begin();
	for (auto it = components.begin(); it != components.end() && rhsIt != rhs.components.end(); ++it, ++rhsIt)
	{
		*it = std::move((*rhsIt)->Clone(this));
		(*it)->SetObject(this);
	}

	auto transform = GetComponent<Transform>();

	if (transform)
		transform.value()->children.clear();

	auto rhsTransform = rhs.GetComponent<Transform>();

	if (rhsTransform)
	{
		for (auto& child : rhsTransform.value()->children)
		{
			childrenObjects.push_back(*child->gameObject);
			childrenObjects.back().GetComponent<Transform>().value()->SetParent(transform.value());
		}
	}
}

GameObject& GameObject::operator=(const GameObject& rhs) noexcept
{
	deleteFlag = rhs.deleteFlag;
	name = rhs.name;
	childrenObjects = rhs.childrenObjects;

	components.resize(rhs.components.size());

	auto rhsIt = rhs.components.begin();
	for (auto it = components.begin(); it != components.end() && rhsIt != rhs.components.end(); ++it, ++rhsIt)
	{
		*it = std::move((*rhsIt)->Clone(this));
		(*it)->SetObject(this);
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
	deleteFlag(std::move(rhs.deleteFlag)),
	name(std::move(rhs.name)),
	components(std::move(rhs.components)),
	childrenObjects(std::move(rhs.childrenObjects))
{
	for (auto& c : components)
		c->SetObject(this);
}

GameObject& GameObject::operator=(GameObject&& rhs) noexcept
{
	deleteFlag = rhs.deleteFlag;
	name = std::move(rhs.name);
	components = std::move(rhs.components);
	childrenObjects = std::move(rhs.childrenObjects);

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

	auto transform = GetComponent<Transform>();

	if (transform)
	{
		for (auto& child : transform.value()->children)
			child->gameObject->Update();
	}
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

GameObject& GameObject::AddChild(const GameObject& child) noexcept
{
	childrenObjects.push_back(child);
	return childrenObjects.back();
}

GameObject& GameObject::AddChild(GameObject&& child) noexcept
{
	childrenObjects.push_back(std::move(child));
	return childrenObjects.back();
}

std::list<GameObject>& GameObject::GetChildren() noexcept
{
	return childrenObjects;
}

GameObject& GameObject::AddToScene(Scene& scene, Transform* parent) noexcept
{
	GameObject& newGameObject = scene.AddSceneObject(*this);

	auto transform = newGameObject.GetComponent<Transform>();

	if (parent != nullptr)
	{
		if (transform)
			transform.value()->SetParent(parent);
	}

	return newGameObject;
}
