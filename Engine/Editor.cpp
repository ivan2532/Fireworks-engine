#include "Editor.hpp"
#include "Engine.hpp"
#include "GLFW/glfw3.h"
#include "ImageLoader.h"
#include <iostream>

Editor::Editor(Engine& eng) noexcept
	:
	engine(eng),
	selectedObject(nullptr),
	nodeIndexCount(0),
	selectedHierarchy(-1),
	translateImage(ImageLoader::TextureFromFile("TranslationButton.png", "EditorIcons")),
	rotateImage(ImageLoader::TextureFromFile("RotationButton.png", "EditorIcons")),
	scaleImage(ImageLoader::TextureFromFile("ScaleButton.png", "EditorIcons"))
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

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, dockspacePadding));
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

	DrawMenu();
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
				if(engine.wnd.GetKey(GLFW_KEY_ENTER, false))
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
			engine.sceneViewAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		}

		ImVec2 topLeft(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y);
		ImVec2 bottomRight(ImGui::GetItemRectMin().x + engine.wnd.GetBufferWidth(), ImGui::GetItemRectMin().y + engine.wnd.GetBufferHeight());

		ImGui::GetWindowDrawList()->AddImage(
			(void*)(intptr_t)engine.wnd.GetColorBuffer(),
			topLeft,
			bottomRight,
			ImVec2(0, 1), ImVec2(1, 0)
		);

		sceneViewFocused = ImGui::IsWindowFocused();
	}
	ImGui::End();
}

void Editor::DrawMenu() noexcept
{
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

		dockspacePadding = ImGui::GetWindowSize().y;

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

	ImGui::SetNextWindowPos(ImVec2(0, dockspacePadding));
	ImGui::SetNextWindowSize(ImVec2(engine.wnd.GetWidth(), buttonSize + style.ItemSpacing.y * 2 + style.WindowPadding.y * 2 + style.FramePadding.y));
	ImGui::Begin("Transform panel", (bool*)0, transformPanelFlags);

	bool pushedColor1 = false;
	if (transformationMode == 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		pushedColor1 = true;
	}
	if (ImGui::ImageButton((void*)(intptr_t)translateImage, ImVec2(buttonSize, buttonSize)))
	{
		transformationMode = 0;
	}
	if (pushedColor1 && transformationMode == 0)
	{
		ImGui::PopStyleColor(3);
	}

	ImGui::SameLine();

	bool pushedColor2 = false;
	if (transformationMode == 1)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		pushedColor2 = true;
	}
	if (ImGui::ImageButton((void*)(intptr_t)rotateImage, ImVec2(buttonSize, buttonSize)))
	{
		transformationMode = 1;
	}
	if (pushedColor2 && transformationMode == 1)
	{
		ImGui::PopStyleColor(3);
	}

	ImGui::SameLine();

	bool pushedColor3 = false;
	if (transformationMode == 2)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		pushedColor3 = true;
	}
	if (ImGui::ImageButton((void*)(intptr_t)scaleImage, ImVec2(buttonSize, buttonSize)))
	{
		transformationMode = 2;
	}
	if (pushedColor3 && transformationMode == 2)
	{
		ImGui::PopStyleColor(3);
	}

	dockspacePadding += buttonSize + style.ItemSpacing.y * 2 + style.WindowPadding.y * 2 + style.FramePadding.y;
	ImGui::End();
}
