#include "AssetExplorerWindow.hpp"
#include "imguiIncludes.hpp"
#include "Editor.hpp"

AssetExplorerWindow::AssetExplorerWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Asset explorer")
{
}

void AssetExplorerWindow::Draw() noexcept
{
	if (!open)
		return;

	if (ImGui::Begin("Asset explorer", &open))
	{
		static float folderSize = ImGui::GetWindowWidth() / 5;
		static float fileSize = ImGui::GetWindowWidth() * 4 / 5;

		editor.Splitter(true, 8.0f, &folderSize, &fileSize, 10.0f, 10.0f);
		ImGui::BeginChild("Folder explorer", ImVec2(folderSize, 0), true);
		ImGui::Text("Folders");
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

		ImGui::BeginChild("File explorer", ImVec2(fileSize, 0), true);
		ImGui::Text("Files");
		ImGui::EndChild();
	}
	ImGui::End();
}
