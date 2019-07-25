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
	engine(rEngine),
	selectedHierarchy(-1),
	translateImage(ImageLoader::TextureFromFile("TranslationButton.png", "EditorIcons")),
	rotateImage(ImageLoader::TextureFromFile("RotationButton.png", "EditorIcons")),
	scaleImage(ImageLoader::TextureFromFile("ScaleButton.png", "EditorIcons")),
	sceneViewAspectRatio(4.0f / 3.0f)
{
	SpawnWindows();
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

	DrawGizmo();
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

	gizmoTransform =
		selectedObject->GetComponent<Transform>().has_value() ?
		selectedObject->GetComponent<Transform>().value() : nullptr;

	if (gizmoTransform != nullptr)
	{
		drawGizmo = true;
	}
	else
	{
		drawGizmo = false;
	}
}

void Editor::DrawMenuBar() noexcept
{
	float menuPadding = 0.0f;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl + N"))
			{

			}

			if (ImGui::MenuItem("New Project", "Ctrl + Shift + N"))
			{

			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::MenuItem("Open Scene", "Ctrl + O"))
			{

			}

			if (ImGui::MenuItem("Open Project", "Ctrl + Shift + O"))
			{

			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::MenuItem("Save scene", "Ctrl + S"))
			{

			}

			if (ImGui::MenuItem("Save Project", "Ctrl + Shift + S"))
			{

			}

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

		menuPadding = ImGui::GetWindowSize().y;
		ImGui::EndMainMenuBar();
	}

	auto style = ImGui::GetStyle();

	auto transformPanelFlags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse;

	float buttonSize = (float)engine.wnd.GetWidth() / (float)engine.wnd.GetHeight() * 15.0f;

	ImGui::SetNextWindowPos(ImVec2(0, menuPadding));
	ImGui::SetNextWindowSize(ImVec2(engine.wnd.GetWidth(), buttonSize + style.ItemSpacing.y * 2 + style.WindowPadding.y * 2 + style.FramePadding.y));
	ImGui::Begin("Transform panel", (bool*)0, transformPanelFlags);

	bool pushedColor1 = false;
	if (currentOperation == ImGuizmo::TRANSLATE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		pushedColor1 = true;
	}
	if (ImGui::ImageButton((void*)(intptr_t)translateImage, ImVec2(buttonSize, buttonSize)))
	{
		currentOperation = ImGuizmo::TRANSLATE;
	}
	if (pushedColor1)
	{
		ImGui::PopStyleColor(3);
	}

	ImGui::SameLine();

	bool pushedColor2 = false;
	if (currentOperation == ImGuizmo::ROTATE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		pushedColor2 = true;
	}
	if (ImGui::ImageButton((void*)(intptr_t)rotateImage, ImVec2(buttonSize, buttonSize)))
	{
		currentOperation = ImGuizmo::ROTATE;
	}
	if (pushedColor2)
	{
		ImGui::PopStyleColor(3);
	}

	ImGui::SameLine();

	bool pushedColor3 = false;
	if (currentOperation == ImGuizmo::SCALE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		pushedColor3 = true;
	}
	if (ImGui::ImageButton((void*)(intptr_t)scaleImage, ImVec2(buttonSize, buttonSize)))
	{
		currentOperation = ImGuizmo::SCALE;
	}
	if (pushedColor3)
	{
		ImGui::PopStyleColor(3);
	}

	ImGui::SameLine();

	if (ImGui::RadioButton("Local", (currentMode == ImGuizmo::LOCAL) ? true : false))
	{
		currentMode = ImGuizmo::LOCAL;
	}

	ImGui::SameLine();

	if (ImGui::RadioButton("World", (currentMode == ImGuizmo::WORLD) ? true : false))
	{
		currentMode = ImGuizmo::WORLD;
	}

	dockspacePadding = menuPadding + buttonSize + style.ItemSpacing.y * 2 + style.WindowPadding.y * 2 + style.FramePadding.y;
	ImGui::End();
}

void Editor::DrawGizmo() noexcept
{
	if (!drawGizmo)
		return;

	if (!ImGuizmo::IsUsing())
	{
		Float4x4 matrix = Math::Glm4x4ToArray(gizmoTransform->GetTransformation());

		for (int i = 0; i < 16; i++)
			gizmoMatrix[i] = matrix.data[i];
	}
	else
	{
		gizmoTransform->SetTransformation(Math::ArrayToGlm4x4(gizmoMatrix), true);
	}

	ImGuiStyle style = ImGui::GetStyle();

	Float4x4 view = Math::Glm4x4ToArray(engine.activeCamera->GetViewMatrix());
	Float4x4 projection = Math::Glm4x4ToArray(engine.activeCamera->GetProjectionMatrix());

	ImGuizmo::SetRect(
		bottomLeftSceneView.x,
		bottomLeftSceneView.y + style.FramePadding.y * 2 + 15.0f,
		topRightSceneView.x - bottomLeftSceneView.x,
		topRightSceneView.y - bottomLeftSceneView.y - style.FramePadding.y * 2 - 15.0f
	);

	ImGuizmo::Manipulate(
		view.data,
		projection.data,
		currentOperation,
		currentOperation == ImGuizmo::SCALE ? ImGuizmo::LOCAL : currentMode,
		gizmoMatrix
	);
}

float Editor::GetSceneViewAspectRatio() const noexcept
{
	return sceneViewAspectRatio;
}
