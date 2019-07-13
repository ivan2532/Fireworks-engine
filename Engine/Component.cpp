#include "Component.hpp"

GameObject* Component::GetObject() const noexcept
{
	return gameObject;
}

void Component::SetObject(GameObject* go) noexcept
{
	gameObject = go;
}
