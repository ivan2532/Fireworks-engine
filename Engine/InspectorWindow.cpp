#include "InspectorWindow.hpp"
#include "imguiIncludes.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include <GLFW\glfw3.h>

InspectorWindow::InspectorWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Inspector")
{
}

void InspectorWindow::Draw() noexcept
{
	if (!open)
		return;

	if (ImGui::Begin("Inspector", &open))
	{
		if (editor.selectedObject)
		{
			char buf[64] = {};
			std::string goName = editor.selectedObject->GetName();

			for (size_t i = 0; i < goName.size() && i < 64; i++)
			{
				buf[i] = goName[i];
			}

			ImGui::Text("Game object name \n(press ENTER to apply new name): ");
			if (ImGui::InputText("##go_name_input", buf, sizeof(buf)))
			{
				if (editor.engine.GetWindow()->GetKey(GLFW_KEY_ENTER, false))
					editor.selectedObject->SetName(buf);
			}

			ImGui::Spacing();

			ImGui::Text("Components: ");
			for (auto& component : editor.selectedObject->components)
			{
				component->DrawInspector();
			}
		}
	}
	ImGui::End();
}
