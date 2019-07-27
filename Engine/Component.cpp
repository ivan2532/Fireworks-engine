#include "Component.hpp"
#include "GameObject.hpp"
#include "Editor.hpp"
#include "ComponentUndoable.hpp"

GameObject* Component::GetObject() const noexcept
{
	return gameObject;
}

void Component::SetObject(GameObject* go) noexcept
{
	gameObject = go;
}