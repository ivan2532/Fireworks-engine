#pragma once
#include <string>

class Editor;

class EditorWindow
{
public:
	EditorWindow(Editor& editor, const std::string& name) noexcept;
public:
	virtual void Draw() noexcept = 0;
public:
	void SetOpen(bool value) noexcept;
protected:
	Editor& editor;
	std::string name;
	bool open;
};