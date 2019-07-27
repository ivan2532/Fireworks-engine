#include "GizmoManager.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include "imguiIncludes.hpp"
#include "Math.hpp"
#include "ImageLoader.h"
#include "ComponentUndoable.hpp"

GizmoManager::GizmoManager(Editor& rEditor) noexcept
	:
	editor(rEditor),
	gizmoMatrix {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	},
	translateImage(ImageLoader::TextureFromFile("TranslationButton.png", "EditorIcons")),
	rotateImage(ImageLoader::TextureFromFile("RotationButton.png", "EditorIcons")),
	scaleImage(ImageLoader::TextureFromFile("ScaleButton.png", "EditorIcons"))
{
	sceneViewWindow = editor.GetEditorWindow<SceneViewWindow>().value();
}

void GizmoManager::DrawGizmo() noexcept
{
	if (!ImGuizmo::IsUsing())
	{
		Float4x4 matrix = Math::Glm4x4ToArray(gizmoTransform->GetTransformation());

		for (int i = 0; i < 16; i++)
			gizmoMatrix[i] = matrix.data[i];

		if (pushUndoable)
		{
			editor.PushUndoable(
				std::move(
					std::make_unique<ComponentUndoable<Transform>>(*gizmoTransform, startState, *gizmoTransform)
				)
			);
			pushUndoable = false;
		}
	}
	else
	{
		if (!pushUndoable)
		{
			startState = *gizmoTransform;
			pushUndoable = true;
		}

		gizmoTransform->SetTransformation(Math::ArrayToGlm4x4(gizmoMatrix), true);
	}

	ImGuiStyle style = ImGui::GetStyle();

	Float4x4 view = Math::Glm4x4ToArray(editor.engine.GetActiveCamera()->GetViewMatrix());
	Float4x4 projection = Math::Glm4x4ToArray(editor.engine.GetActiveCamera()->GetProjectionMatrix());

	ImGuizmo::SetRect(
		sceneViewWindow->GetBottomLeft().x,
		sceneViewWindow->GetBottomLeft().y + style.FramePadding.y * 2 + 15.0f,
		sceneViewWindow->GetTopRight().x - sceneViewWindow->GetBottomLeft().x,
		sceneViewWindow->GetTopRight().y - sceneViewWindow->GetBottomLeft().y - style.FramePadding.y * 2 - 15.0f
	);

	ImGuizmo::Manipulate(
		view.data,
		projection.data,
		currentOperation,
		currentOperation == ImGuizmo::SCALE ? ImGuizmo::LOCAL : currentMode,
		gizmoMatrix
	);
}

void GizmoManager::DrawTransformationMenu() noexcept
{
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

	float buttonSize = (float)editor.engine.GetWindow()->GetWidth() / (float)editor.engine.GetWindow()->GetHeight() * 15.0f;

	ImGui::SetNextWindowPos(ImVec2(0, editor.menuPadding));
	ImGui::SetNextWindowSize(ImVec2(editor.engine.GetWindow()->GetWidth(), buttonSize + style.ItemSpacing.y * 2 + style.WindowPadding.y * 2 + style.FramePadding.y));
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

	editor.dockspacePadding = editor.menuPadding + buttonSize + style.ItemSpacing.y * 2 + style.WindowPadding.y * 2 + style.FramePadding.y;
	ImGui::End();
}
