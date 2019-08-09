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
		/*for (int i = 0; i < (int)editor.engine.activeScene->sceneObjects.size(); i++)
		{
			auto transform = editor.engine.activeScene->sceneObjects[i].GetComponent<Transform>();

			if (!transform)
				continue;

			if (transform.value()->GetParent() == nullptr && !editor.engine.activeScene->sceneObjects[i].GetComponent<SceneCameraController>())
			{
				transform.value()->DrawHierarchy(editor, nodeIndexCount, std::to_string(i));
			}
		}*/
		int indexCounter = 0;
		for (auto it = editor.engine.activeScene->sceneObjects.begin(); it != editor.engine.activeScene->sceneObjects.end(); ++it, indexCounter++)
		{
			auto transform = it->GetComponent<Transform>();

			if (!transform)
				continue;

			if (transform.value()->GetParent() == nullptr && !it->GetComponent<SceneCameraController>())
				transform.value()->DrawHierarchy(editor, nodeIndexCount, std::to_string(indexCounter));
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
