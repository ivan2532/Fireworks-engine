#include "AssetExplorerWindow.hpp"
#include "imguiIncludes.hpp"
#include "Editor.hpp"

namespace fs = std::filesystem;

AssetExplorerWindow::AssetExplorerWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Asset explorer"),
	assetManager("D:\\Fireworks projects\\Test project\\Assets")
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
		DrawAssetsTree();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("File explorer", ImVec2(fileSize, 0), true);
		ImGui::Text("Files");
		ImGui::EndChild();
	}
	ImGui::End();
}

void AssetExplorerWindow::DrawAssetsTree() noexcept
{
	DrawFolderTree(assetManager.folders.front());
}

void AssetExplorerWindow::DrawFolderTree(const Folder& folder) noexcept
{
	if (ImGui::TreeNode(folder.name.c_str()))
	{
		for (const auto& childFolder : folder.childrenIndices)
		{
			DrawFolderTree(assetManager.folders[childFolder]);
		}

		ImGui::TreePop();
	}
}
