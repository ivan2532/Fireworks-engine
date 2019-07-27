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

template<class T>
void Component::PushUndoable(Editor& editor, void* target, void* startState, void* endState) noexcept
{
	auto newUndoable = std::make_unique<ComponentUndoable<T>>(
		reinterpret_cast<T*>(target),
		*(reinterpret_cast<T*>(startState)),
		*(reinterpret_cast<T*>(endState))
	);

	editor.PushUndoable(std::move(newUndoable));
}

template void Component::PushUndoable<Transform>(Editor& editor, void* target, void* startState, void* endState) noexcept;