#include "HierarchyWindow.hpp"
#include "imguiIncludes.hpp"
#include "imgui/imgui_internal.h"
#include "Engine.hpp"
#include "Transform.hpp"
#include "SceneCameraController.hpp"
#include "GizmoManager.hpp"
#include <iostream>

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
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAGGABLE_MODEL"))
			{
				Model* droppedModel = (Model*)payload->Data;
				droppedModel->LoadCPU();
				droppedModel->LoadGPU();
				droppedModel->AddToScene(*editor.engine.activeScene);
			}

			ImGui::EndDragDropTarget();
		}

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
