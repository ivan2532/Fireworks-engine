#pragma once
#include <string>

class Editor;

class EditorWindow
{
public:
	EditorWindow(Editor& editor, const std::string& name) noexcept;
public:
	virtual void Draw() noexcept = 0;
	virtual void ProcessInput() noexcept {};
public:
	std::string GetName() const noexcept;
	bool GetOpen() const noexcept;
	void SetOpen(bool value) noexcept;
protected:
	Editor& editor;
	std::string name;
	bool open = true;
};