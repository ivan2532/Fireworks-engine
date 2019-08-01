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
	assetManager(editor, "D:\\Fireworks projects\\Test project\\Assets", editor.engine.defaultShader)
{
}

void AssetExplorerWindow::Draw() noexcept
{
	//ImGui::ShowDemoWindow();
	if (!open)
		return;

	if (ImGui::Begin("Asset explorer", &open))
	{
		static bool initializedWidths = false;

		if (!initializedWidths)
		{
			folderWidth = ImGui::GetWindowWidth() / 5;
			fileWidth = ImGui::GetWindowWidth() * 4 / 5;
			initializedWidths = true;
		}

		editor.Splitter(true, 8.0f, &folderWidth, &fileWidth, 10.0f, 10.0f);

		ImGui::BeginChild("Folder explorer", ImVec2(folderWidth, 0), true);
		ImGui::Text("Folders");
		ImGui::Separator();
		ImGui::Spacing();

		DrawAssetsTree();

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("File explorer", ImVec2(fileWidth, 0), true);
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
	const auto columnCount = std::max(1, static_cast<int>(fileWidth / (fileButtonSize + style.ItemSpacing.x)));
	
	for (unsigned i = 0; i < folder.assets.size(); i++)
	{
		auto curAsset = folder.assets[i].get();

		std::ostringstream columnID;
		columnID << "##file_" << i << "_column";
		ImGui::Columns(columnCount, columnID.str().c_str());

		ImGui::PushID(i);
		ImGui::Button("", ImVec2(fileButtonSize, fileButtonSize));
		ImGui::PopID();

		std::string textString = curAsset->GetName();
		bool cutString = false;

		if (ImGui::CalcTextSize(textString.c_str()).x > fileButtonSize)
			cutString = true;

		if (cutString)
		{
			textString.pop_back();
			textString += "...";

			while (textString.length() > 3 && ImGui::CalcTextSize(textString.c_str()).x > fileButtonSize)
			{
				cutString = true;
				textString.erase(textString.end() - 4);
			}
		}

		ImGui::Text(textString.c_str());

		ImGui::NextColumn();
	}
}
