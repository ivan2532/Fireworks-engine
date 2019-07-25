#include "SceneViewWindow.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include "GLFW/glfw3.h"

SceneViewWindow::SceneViewWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Scene view")
{
}

void SceneViewWindow::Draw() noexcept
{
	editor.engine.GetWindow()->UnbindFramebuffer();

	if (!open)
	{
		editor.drawGizmo = false;
		return;
	}

	if (ImGui::Begin("Scene view", &open))
	{
		if (ImGui::IsWindowHovered()) //Focus on right-click
		{
			if (editor.engine.GetWindow()->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT, false))
			{
				if (!editor.sceneViewFocused)
					ImGui::SetWindowFocus();
			}
		}

		auto windowSize = ImGui::GetWindowSize();
		auto bufferWidth = editor.engine.GetWindow()->GetBufferWidth();
		auto bufferHeight = editor.engine.GetWindow()->GetBufferHeight();

		if (windowSize.x != bufferWidth || windowSize.y != bufferHeight)
		{
			editor.engine.GetWindow()->MakeFramebuffer(windowSize.x, windowSize.y);
			editor.sceneViewAspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
		}

		editor.bottomLeftSceneView = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		editor.topRightSceneView = ImVec2(editor.bottomLeftSceneView.x + ImGui::GetWindowSize().x,
			editor.bottomLeftSceneView.y + ImGui::GetWindowSize().y);

		ImGui::GetWindowDrawList()->AddImage(
			(void*)(intptr_t)editor.engine.GetWindow()->GetColorBuffer(),
			editor.bottomLeftSceneView,
			editor.topRightSceneView,
			ImVec2(0, 1), ImVec2(1, 0)
		);

		editor.sceneViewFocused = ImGui::IsWindowFocused();
	}
	ImGui::End();
}