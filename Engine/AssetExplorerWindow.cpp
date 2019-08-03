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
			DrawFolderContents(selectedFolderIndex);

		ImGui::EndChild();
	}
	ImGui::End();
}

void AssetExplorerWindow::DrawAssetsTree() noexcept
{
	nodeIndexCount = 0;
	DrawFolderTree(0);
}

void AssetExplorerWindow::DrawFolderTree(int folderIndex) noexcept
{
	int currentIndex = nodeIndexCount++;
	auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (assetManager.folders[folderIndex].childrenIndices.size() == 0)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (currentIndex == selectedFolderID)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (currentIndex == 0)
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

	auto icon = ICON_FA_FOLDER;

	if (assetManager.folders[folderIndex].expanded)
		icon = ICON_FA_FOLDER_OPEN;

	std::ostringstream displayString;
	displayString << icon << "  " << assetManager.folders[folderIndex].name;
	bool expandedNode = ImGui::TreeNodeEx((void*)(intptr_t)currentIndex, nodeFlags, displayString.str().c_str());
	
	if (ImGui::IsItemClicked())
	{
		selectedFolderID = currentIndex;
		selectedFolderIndex = folderIndex;
	}

	if (expandedNode)
	{
		if (assetManager.folders[folderIndex].childrenIndices.size() > 0)
			assetManager.folders[folderIndex].expanded = true;
		else
			assetManager.folders[folderIndex].expanded = false;

		for (const auto& childFolder : assetManager.folders[folderIndex].childrenIndices)
			DrawFolderTree(childFolder);

		ImGui::TreePop();
	}
	else
		assetManager.folders[folderIndex].expanded = false;
}

void AssetExplorerWindow::DrawFolderContents(int folderIndex) const noexcept
{
	ImGuiStyle& style = ImGui::GetStyle();
	const auto columnCount = std::max(1, static_cast<int>(fileWidth / (fileButtonSize + style.ItemSpacing.x)));
	
	
	for (unsigned i = 0; i < assetManager.folders[folderIndex].assets.size(); i++)
	{
		std::ostringstream columnID;
		columnID << "##file_" << i << "_column";
		ImGui::Columns(columnCount, columnID.str().c_str());

		ImGui::PushID(i);
		//ImGui::Button("", ImVec2(fileButtonSize, fileButtonSize));
		ImGui::ImageButton((void*)(intptr_t)assetManager.folders[folderIndex].assets[i]->GetPreview(),
			ImVec2(fileButtonSize, fileButtonSize));
		ImGui::PopID();

		std::string textString = assetManager.folders[folderIndex].assets[i]->GetName();

		ImGui::TextWrapped(textString.c_str());

		ImGui::NextColumn();
	}
}
