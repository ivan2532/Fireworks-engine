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
		if (ImGui::TreeNode("Assets"))
		{
			DrawAssetsTree();
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

void AssetExplorerWindow::DrawAssetsTree() noexcept
{
	nodeIndexCount = 0;
	DrawDirectoryTree(assetManager.GetAssetsDir());
}

void AssetExplorerWindow::DrawDirectoryTree(const std::filesystem::path& directory) noexcept
{
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_directory(entry.status()))
			{
				auto directoryString = entry.path().filename().string();

				auto currentIndex = nodeIndexCount++;
				auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

				if (currentIndex == selectedFolderID)
					nodeFlags |= ImGuiTreeNodeFlags_Selected;

				bool isLeaf = true;
				for (auto& leafCheck : fs::directory_iterator(entry.path()))
				{
					isLeaf = false;
					break;
				}

				if(isLeaf)
					nodeFlags |= ImGuiTreeNodeFlags_Leaf;

				bool open = ImGui::TreeNodeEx((void*)(intptr_t)currentIndex, nodeFlags, directoryString.c_str());

				if (ImGui::IsItemClicked())
				{
					selectedFolderID = currentIndex;
					selectedFolder = entry.path();
				}

				if (open)
				{
					DrawDirectoryTree(entry.path());
					ImGui::TreePop();
				}
			}
		}
	}
}
