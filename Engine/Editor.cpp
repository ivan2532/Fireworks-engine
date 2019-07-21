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

void Editor::DrawDockSpace() noexcept
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Dockspace", (bool*)0, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

void Editor::DrawGUI() noexcept
{
	DrawDockSpace();
	DrawHierarchyUI();
	DrawInspectorUI();
	DrawAssetExplorerUI();
	DrawSceneView();
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
	if (ImGui::Begin("Asset explorer"))
	{
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

void Editor::DrawSceneView() noexcept
{
	engine.wnd.UnbindFramebuffer();
	
	if (ImGui::Begin("Scene view"))
	{
		auto windowSize = ImGui::GetWindowSize();
		auto bufferWidth = engine.wnd.GetBufferWidth();
		auto bufferHeight = engine.wnd.GetBufferHeight();
		
		if (windowSize.x != bufferWidth || windowSize.y != bufferHeight)
		{
			engine.wnd.MakeFramebuffer(windowSize.x, windowSize.y);
		}

		ImVec2 topLeft(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y);
		ImVec2 bottomRight(ImGui::GetItemRectMin().x + engine.wnd.GetBufferWidth(), ImGui::GetItemRectMin().y + engine.wnd.GetBufferHeight());

		ImGui::GetWindowDrawList()->AddImage(
			(void*)(intptr_t)engine.wnd.GetColorBuffer(),
			topLeft,
			bottomRight,
			ImVec2(0, 1), ImVec2(1, 0)
		);
	}
	ImGui::End();
}
