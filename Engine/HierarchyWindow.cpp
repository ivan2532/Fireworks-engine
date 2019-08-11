#include "HierarchyWindow.hpp"
#include "imguiIncludes.hpp"
#include "imgui/imgui_internal.h"
#include "Engine.hpp"
#include "Transform.hpp"
#include "SceneCameraController.hpp"
#include "GizmoManager.hpp"
#include "InspectorWindow.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

HierarchyWindow::HierarchyWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Hierarchy")
{
	auto search = editor.GetEditorWindow<InspectorWindow>();

	if (search)
		inspectorWindow = search.value();
	else
		std::cout << "ERROR: No inspector window at time of hierarchy window creation." << std::endl;
}

void HierarchyWindow::Draw() noexcept
{
	if (!open)
		return;

	nodeIndexCount = 0;

	if (ImGui::Begin("Hierarchy", &open))
	{
		windowFocused = ImGui::IsWindowFocused();

		ImRect windowContentRect(
			ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x,
			ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y,
			ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x,
			ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y
		);

		ImGui::PushClipRect(
			ImVec2(0, 0),
			ImVec2((float)editor.GetMainWindow()->GetWidth(), (float)editor.GetMainWindow()->GetHeight()),
			false
		);

		ImGui::PushID(1);
		if (ImGui::BeginDragDropTargetCustom(windowContentRect, 1))
		{
			const ImGuiPayload* payload = nullptr;

			if (payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAGGABLE_MODEL"))
			{
				struct ModelWrapper
				{
					Model* pModel;
				};

				Model* droppedModel = reinterpret_cast<ModelWrapper*>(payload->Data)->pModel;
				droppedModel->LoadCPU();
				droppedModel->LoadGPU();
				droppedModel->GetObject().AddToScene(*editor.GetEngine().activeScene);
			}
			else if (payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAGGABLE_TRANSFORM"))
			{
				struct TransformWrapper
				{
					Transform* pModel;
				};

				Transform* droppedTransformer = reinterpret_cast<TransformWrapper*>(payload->Data)->pModel;
				droppedTransformer->SetParent(nullptr);
			}

			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();
		ImGui::PopClipRect();

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

void HierarchyWindow::ProcessInput() noexcept
{
	if (!windowFocused)
		return;
	
	if (inspectorWindow->selectedObject && editor.GetMainWindow()->GetKeyDown(GLFW_KEY_DELETE, false))
	{
		inspectorWindow->selectedObject->Delete();
		inspectorWindow->selectedObject = nullptr;
	}
}

int HierarchyWindow::GetSelectedHierarchy() const noexcept
{
	return selectedHierarchy;
}

void HierarchyWindow::SetSelectedHierarchy(int value) noexcept
{
	selectedHierarchy = value;
}
