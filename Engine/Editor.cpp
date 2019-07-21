#include "Editor.hpp"
#include "Engine.hpp"
#include "GLFW/glfw3.h"
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
	DrawAssetExplorerUI();
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
			char buf[64] = {};
			std::string goName = selectedObject->GetName();

			for (size_t i = 0; i < goName.size() && i < 64; i++)
			{
				buf[i] = goName[i];
			}

			ImGui::Text("Game object name \n(press ENTER to apply new name): ");
			if (ImGui::InputText("##go_name_input", buf, sizeof(buf)))
			{
				if(engine.wnd.GetKey(GLFW_KEY_ENTER))
					selectedObject->SetName(buf);
			}

			ImGui::Spacing();

			ImGui::Text("Components: ");
			for (auto& component : selectedObject->components)
			{
				component->DrawInspector();
			}
		}
	}
	ImGui::End();
}

void Editor::DrawAssetExplorerUI() noexcept
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	// I set size...

	auto flags =
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove;

	//::ShowDemoWindow();
	if (ImGui::Begin("Asset explorer", (bool*) 0, flags))
	{
		/*if (ImGui::BeginTabBar("View"))
		{
			if (ImGui::BeginTabItem("Filter explorer"))
			{
				ImGui::Text("Filters");
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("File explorer"))
			{
				ImGui::Text("Files");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}*/
		ImGui::BeginChild("Filter explorer", ImVec2(ImGui::GetWindowWidth() / 5, 0), true);
		ImGui::Text("Filters");
		if (ImGui::TreeNode("Root"))
		{
			for (int i = 0; i < 10; i++)
			{
				if (ImGui::TreeNode((void*)(intptr_t)i, "Filter %d", i))
				{
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("File explorer", ImVec2(0, 0), true);
		ImGui::Text("Files");
		ImGui::EndChild();
	}
	ImGui::End();
}
