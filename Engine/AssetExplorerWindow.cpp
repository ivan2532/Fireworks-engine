#include "AssetExplorerWindow.hpp"
#include "imguiIncludes.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include "FontIcons.hpp"
#include <sstream>

namespace fs = std::filesystem;

AssetExplorerWindow::AssetExplorerWindow(Editor& editor) noexcept
	:
	EditorWindow(editor, "Asset explorer"),
	assetManager("D:\\Fireworks projects\\Test project\\Assets", editor.engine.defaultShader)
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
		ImGui::Separator();
		ImGui::Spacing();

		DrawAssetsTree();

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("File explorer", ImVec2(fileSize, 0), true);
		ImGui::Text("Files");
		ImGui::Separator();
		ImGui::Spacing();

		if (selectedFolderID != -1)
		{
			DrawFolderContents(*selectedFolder);
		}

		ImGui::EndChild();
	}
	ImGui::End();
}

void AssetExplorerWindow::DrawAssetsTree() noexcept
{
	nodeIndexCount = 0;
	DrawFolderTree(assetManager.folders.front());
}

void AssetExplorerWindow::DrawFolderTree(FolderNode& folder) noexcept
{
	int currentIndex = nodeIndexCount++;
	auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (folder.childrenIndices.size() == 0)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (currentIndex == selectedFolderID)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (currentIndex == 0)
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

	auto icon = ICON_FA_FOLDER;

	if (folder.expanded)
		icon = ICON_FA_FOLDER_OPEN;

	std::ostringstream displayString;
	displayString << icon << "  " << folder.name;
	bool expandedNode = ImGui::TreeNodeEx((void*)(intptr_t)currentIndex, nodeFlags, displayString.str().c_str());
	
	if (ImGui::IsItemClicked())
	{
		selectedFolderID = currentIndex;
		selectedFolder = &folder;
	}

	if (expandedNode)
	{
		if (folder.childrenIndices.size() > 0)
			folder.expanded = true;
		else
			folder.expanded = false;

		for (const auto& childFolder : folder.childrenIndices)
			DrawFolderTree(assetManager.folders[childFolder]);

		ImGui::TreePop();
	}
	else
		folder.expanded = false;
}

void AssetExplorerWindow::DrawFolderContents(const FolderNode& folder) const noexcept
{
	ImGuiStyle& style = ImGui::GetStyle();
	float xLimit = ImGui::GetWindowPos().x + ImGui::GetContentRegionMax().x;

	for (unsigned i = 0; i < folder.assets.size(); i++)
	{
		auto curAsset = folder.assets[i].get();

		ImGui::PushID(i);

		ImGui::Button(curAsset->GetName().c_str(), ImVec2(fileButtonSize, fileButtonSize));
		float lastButtonX = ImGui::GetItemRectMax().x;
		float nextButtonX = lastButtonX + style.ItemSpacing.x + 50;

		if (i + 1 < folder.assets.size() && nextButtonX < xLimit)
			ImGui::SameLine();

		ImGui::PopID();
	}
}
