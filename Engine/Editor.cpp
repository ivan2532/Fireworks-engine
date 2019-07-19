#include "Editor.hpp"
#include "Engine.hpp"
#include <iostream>

Editor::Editor(Engine& eng) noexcept
	:
	engine(eng),
	selectedObject(nullptr),
	nodeIndexCount(0),
	selectedHierarchy(-1)
{
}

void Editor::DrawGUI() noexcept
{
	DrawHierarchyUI();
	DrawInspectorUI();
}

void Editor::DrawHierarchyUI() noexcept
{
	nodeIndexCount = 0;

	if (ImGui::Begin("Hierarchy"))
	{
		for (int i = 0; i < engine.activeScene->sceneObjects.size(); i++)
		{
			auto transform = engine.activeScene->sceneObjects[i]->GetComponent<Transform>().value();

			if (transform->GetParent() == nullptr && !engine.activeScene->sceneObjects[i]->GetComponent<SceneCameraController>().has_value())
			{
				transform->DrawHierarchy(*this, nodeIndexCount);
			}
		}
	}
	ImGui::End();
}

int Editor::GetSelectedHierarchy() const noexcept
{
	return selectedHierarchy;
}

void Editor::SetSelectedHierarchy(int value) noexcept
{
	selectedHierarchy = value;
}

GameObject* Editor::GetSelectedObject() const noexcept
{
	return selectedObject;
}

void Editor::SetSelectedObject(GameObject* value) noexcept
{
	selectedObject = value;
}

void Editor::DrawInspectorUI() noexcept
{
	//ImGui::ShowDemoWindow();
	if (ImGui::Begin("Inspector"))
	{
		if (selectedObject)
		{
			for (auto& component : selectedObject->components)
			{
				component->DrawInspector();
			}
		}
	}
	ImGui::End();
}
