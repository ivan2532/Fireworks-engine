#include "EditorWindow.hpp"
#include "Editor.hpp"

EditorWindow::EditorWindow(Editor& rEditor, const std::string& rName) noexcept
	:
	editor(rEditor),
	name(rName)
{
}

std::string EditorWindow::GetName() const noexcept
{
	return name;
}

bool EditorWindow::GetOpen() const noexcept
{
	return open;
}

void EditorWindow::SetOpen(bool value) noexcept
{
	open = value;
}
