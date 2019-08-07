#include <GLFW\glfw3.h>
#include "InspectorWindow.hpp"
#include "imguiIncludes.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "GizmoManager.hpp"

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
		if (selectedObject)
		{
			ImGui::Text("Remember to press 'ENTER' to apply changes");
			char buf[64] = {};
			std::string goName = selectedObject->GetName();

			for (size_t i = 0; i < goName.size() && i < 64; i++)
			{
				buf[i] = goName[i];
			}

			ImGui::Text("Game object name: ");
			if (ImGui::InputText("##go_name_input", buf, sizeof(buf)))
			{
				if (editor.engine.GetWindow()->GetKey(GLFW_KEY_ENTER, false))
					selectedObject->SetName(buf);
			}

			ImGui::Spacing();

			ImGui::Text("Components: ");
			for (auto& component : selectedObject->components)
			{
				component->DrawInspector(editor);
			}
		}
	}
	ImGui::End();
}

GameObject* InspectorWindow::GetSelectedObject() const noexcept
{
	return selectedObject;
}

void InspectorWindow::SetSelectedObject(GameObject* value) noexcept
{
	selectedObject = value;

	auto gizmoManager = editor.gizmoManager.get();

	auto transform = selectedObject->GetComponent<Transform>();

	gizmoManager->gizmoTransform =
		transform ?
		transform.value() : nullptr;

	if (gizmoManager->gizmoTransform != nullptr)
	{
		editor.drawGizmo = true;
	}
	else
	{
		editor.drawGizmo = false;
	}
}
