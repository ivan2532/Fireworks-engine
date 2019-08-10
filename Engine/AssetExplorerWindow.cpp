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
	assetManager(editor, "C:\\Fireworks projects\\Test project\\Assets", editor.engine.defaultShader)
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
			DrawFolderContents(selectedFolder);

		ImGui::EndChild();
	}
	ImGui::End();
}

void AssetExplorerWindow::DrawAssetsTree() noexcept
{
	nodeIndexCount = 0;
	DrawFolderTree(&assetManager.folders.front(), "0");
}

void AssetExplorerWindow::DrawFolderTree(FolderNode* folder, const std::string& idBuildUp) noexcept
{
	int currentIndex = nodeIndexCount++;
	auto nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	auto id = idBuildUp;

	if (folder->children.size() == 0)
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;

	if (currentIndex == selectedFolderID)
		nodeFlags |= ImGuiTreeNodeFlags_Selected;

	if (currentIndex == 0)
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

	auto icon = ICON_FA_FOLDER;

	if (folder->expanded)
		icon = ICON_FA_FOLDER_OPEN;

	std::ostringstream displayString;
	displayString << icon << "  " << folder->name;
	bool expandedNode = ImGui::TreeNodeEx(id.c_str(), nodeFlags, displayString.str().c_str());
	
	if (ImGui::IsItemClicked())
	{
		selectedFolderID = currentIndex;
		selectedFolder = folder;
	}

	if (expandedNode)
	{
		if (folder->children.size() > 0)
			folder->expanded = true;
		else
			folder->expanded = false;

		int indexCounter = 0;
		for (auto it = folder->children.begin(); it != folder->children.end(); ++it, indexCounter++)
			DrawFolderTree(*it, id + std::to_string(indexCounter));

		ImGui::TreePop();
	}
	else
		folder->expanded = false;
}

void AssetExplorerWindow::DrawFolderContents(FolderNode* folder) const noexcept
{
	ImGuiStyle& style = ImGui::GetStyle();
	const auto columnCount = std::max(1, static_cast<int>(fileWidth /
		(fileButtonSize + style.ItemSpacing.x + style.FramePadding.x + style.ItemInnerSpacing.x * 2)));
	
	int indexCounter = 0;
	for (auto it = folder->assets.begin(); it != folder->assets.end(); ++it, indexCounter++)
	{
		std::ostringstream columnID;
		columnID << "##file_" << indexCounter << "_column";
		ImGui::Columns(columnCount, columnID.str().c_str());

		if (!(*it))
			break;

		ImGui::PushID(indexCounter);

		ImGui::ImageButton((void*)(intptr_t)(*it)->GetPreview(),
			ImVec2(fileButtonSize, fileButtonSize),
			ImVec2(0, 1), //Flip UVs
			ImVec2(1, 0),
			-1
		);

		if (auto check = dynamic_cast<Model*>(it->get()))
		{
			if (ImGui::BeginDragDropSource())
			{
				struct ModelWrapper
				{
					Model* pModel;
				} data { check };

				ImGui::SetDragDropPayload("HIERARCHY_DRAGGABLE_MODEL", &data, sizeof(ModelWrapper));
				ImGui::EndDragDropSource();
			}
		}

		ImGui::PopID();

		if (!(*it))
			break;
		std::string textString = (*it)->GetName();

		ImGui::TextWrapped(textString.c_str());

		ImGui::NextColumn();
	}
}
