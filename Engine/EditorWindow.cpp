#include "EditorWindow.hpp"
#include "Editor.hpp"

EditorWindow::EditorWindow(Editor& rEditor, const std::string& rName) noexcept
	:
	editor(rEditor),
	name(rName)
{
}

void EditorWindow::SetOpen(bool value) noexcept
{
	open = value;
}
