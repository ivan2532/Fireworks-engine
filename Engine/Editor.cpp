#include "Editor.hpp"
#include "Engine.hpp"
#include "GLFW/glfw3.h"
#include "ImageLoader.h"
#include "glmIncludes.hpp"
#include "imguiIncludes.hpp"
#include "imgui/imgui_internal.h"
#include "Math.hpp"
#include <iostream>

Editor::Editor(Engine& rEngine) noexcept
	:
	engine(rEngine)
{
	SpawnWindows();
	gizmoManager = std::make_unique<GizmoManager>(*this);
}

void Editor::SpawnWindows() noexcept
{
	editorWindows.push_back(std::make_unique<HierarchyWindow>(*this));
	editorWindows.push_back(std::make_unique<InspectorWindow>(*this));
	editorWindows.push_back(std::make_unique<AssetExplorerWindow>(*this));
	editorWindows.push_back(std::make_unique<SceneViewWindow>(*this));
}

bool Editor::Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size) noexcept
{
	using namespace ImGui;
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;
	auto add = (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
	bb.Min = ImVec2(window->DC.CursorPos.x + add.x, window->DC.CursorPos.y + add.y);
	auto itemSize = CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	bb.Max = ImVec2(bb.Min.x + itemSize.x, bb.Min.y + itemSize.y);
	return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

void Editor::DrawDockSpace() noexcept
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + dockspacePadding));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - dockspacePadding));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Dockspace", (bool*)0, window_flags);
	ImGui::PopStyleVar();

	ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
	ImGui::Begin("BackgroundFiller", (bool*)0, window_flags);
	ImGui::End();
}

void Editor::DrawGUI() noexcept
{
	//Capture input
	if (sceneViewFocused || gameViewFocused)
	{
		ImGui::CaptureKeyboardFromApp(false);
		ImGui::CaptureMouseFromApp(false);
	}
	else
	{
		ImGui::CaptureKeyboardFromApp(true);
		ImGui::CaptureMouseFromApp(true);
	}

	DrawDockSpace();
	DrawMenuBar();

	for (auto& window : editorWindows)
		window->Draw();

	if(drawGizmo)
		gizmoManager->DrawGizmo();
}

void Editor::DrawMenuBar() noexcept
{
	menuPadding = 0.0f;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl + N")) { }

			if (ImGui::MenuItem("New Project", "Ctrl + Shift + N")) { }

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::MenuItem("Open Scene", "Ctrl + O")) { }

			if (ImGui::MenuItem("Open Project", "Ctrl + Shift + O")) { }

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::MenuItem("Save scene", "Ctrl + S")) { }

			if (ImGui::MenuItem("Save Project", "Ctrl + Shift + S")) { }

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::MenuItem("Quit"))
			{
				glfwTerminate();
				exit(0);
			}

			ImGui::EndMenu();
		}
		
		ImGui::Separator();

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl + Z"))
				Undo();

			if (ImGui::MenuItem("Redo", "Ctrl + Y"))
				Redo();

			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Window"))
		{
			for (auto& window : editorWindows)
			{
				if (ImGui::MenuItem(window->GetName().c_str(), (const char*)0, window->GetOpen()))
				{
					window->SetOpen(!window->GetOpen());
				}
			}

			ImGui::EndMenu();
		}

		menuPadding = ImGui::GetWindowSize().y;
		ImGui::EndMainMenuBar();
	}

	gizmoManager->DrawTransformationMenu();
}

void Editor::ProcessInput() noexcept
{
	if (engine.wnd.GetKey(GLFW_KEY_LEFT_CONTROL) && engine.wnd.GetKeyDown(GLFW_KEY_Z))
		Undo();
	if (engine.wnd.GetKey(GLFW_KEY_LEFT_CONTROL) && engine.wnd.GetKeyDown(GLFW_KEY_Y))
		Redo();
}

void Editor::PushUndoable(std::unique_ptr<Undoable> newUndoable) noexcept
{
	if (undoBuffer.size() == undoRedoBufferSize)
		undoBuffer.erase(undoBuffer.begin());

	undoBuffer.push_back(std::move(newUndoable));
}

void Editor::Undo() noexcept
{
	if (undoBuffer.size() < 1)
		return;

	undoBuffer.back()->Undo();
	redoBuffer.push_back(std::move(undoBuffer.back()));
	undoBuffer.pop_back();
}

void Editor::Redo() noexcept
{
	if (redoBuffer.size() < 1)
		return;

	redoBuffer.back()->Redo();
	undoBuffer.push_back(std::move(redoBuffer.back()));
	redoBuffer.pop_back();
}

Window* Editor::GetMainWindow() const noexcept
{
	return engine.GetWindow();
}
