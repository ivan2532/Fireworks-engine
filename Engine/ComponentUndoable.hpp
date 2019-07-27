#pragma once
#include "Undoable.hpp"

template<class T>
class ComponentUndoable : public Undoable
{
public:
	ComponentUndoable(T* target, T startState, T endState) noexcept;
	void Undo() noexcept override;
	void Redo() noexcept override;
private:
	T* target;
	T undoneState;
	T redoneState;
};

template<class T>
ComponentUndoable<T>::ComponentUndoable(T* tar, T sState, T eState) noexcept
	:
	target(tar),
	undoneState(sState),
	redoneState(eState)
{
}

template<class T>
void ComponentUndoable<T>::Undo() noexcept
{
	*target = undoneState;
}

template<class T>
void ComponentUndoable<T>::Redo() noexcept
{
	*target = redoneState;
}
