#pragma once

class Undoable
{
public:
	Undoable() = default;
	virtual void Undo() noexcept = 0;
	virtual void Redo() noexcept = 0;
};