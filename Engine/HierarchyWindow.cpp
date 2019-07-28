#include "HierarchyWindow.hpp"
#include "imguiIncludes.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "SceneCameraController.hpp"
#include "GizmoManager.hpp"

HierarchyWindow::HierarchyWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Hierarchy")
{
}

void HierarchyWindow::Draw() noexcept
{
	if (!open)
		return;

	nodeIndexCount = 0;
	if (ImGui::Begin("Hierarchy", &open))
	{
		for (int i = 0; i < (int)editor.engine.activeScene->sceneObjects.size(); i++)
		{
			auto transform = editor.engine.activeScene->sceneObjects[i].GetComponent<Transform>();

			if (!transform.has_value())
				continue;

			if (transform.value()->GetParent() == nullptr && !editor.engine.activeScene->sceneObjects[i].GetComponent<SceneCameraController>().has_value())
			{
				transform.value()->DrawHierarchy(editor, nodeIndexCount);
			}
		}
	}
	ImGui::End();
}

int HierarchyWindow::GetSelectedHierarchy() const noexcept
{
	return selectedHierarchy;
}

void HierarchyWindow::SetSelectedHierarchy(int value) noexcept
{
	selectedHierarchy = value;
}
