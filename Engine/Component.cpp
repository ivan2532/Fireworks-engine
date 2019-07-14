#include "Component.hpp"

Component::Component(GameObject* go) noexcept
	:
	gameObject(go)
{
}

GameObject* Component::GetObject() const noexcept
{
	return gameObject;
}