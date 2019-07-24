#include "AssetExplorerWindow.hpp"
#include "imguiIncludes.hpp"

AssetExplorerWindow::AssetExplorerWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Asset explorer")
{
}

void AssetExplorerWindow::Draw() noexcept
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
