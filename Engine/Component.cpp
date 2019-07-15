#include "Component.hpp"
#include "GameObject.hpp"

Component::Component(GameObject* go) noexcept
	:
	gameObject(go)
{
}

GameObject* Component::GetObject() const noexcept
{
	return gameObject;
}

void Component::OnMouseMove(float x, float y) noexcept
{
}
