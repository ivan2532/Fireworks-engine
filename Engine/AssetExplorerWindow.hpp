#pragma once
#include "EditorWindow.hpp"
#include "AssetManager.hpp"
#include <filesystem>

class Engine;

class AssetExplorerWindow : public EditorWindow
{
public:
	AssetExplorerWindow(Editor& editor) noexcept;
public:
	void Draw() noexcept override;
	void DrawAssetsTree() noexcept;
private:
	void DrawFolderTree(FolderNode* folder, const std::string& idBuildUp) noexcept;
	void DrawFolderContents(FolderNode* folder) const noexcept;
private:
	AssetManager assetManager;
	float folderWidth;
	float fileWidth;
	int nodeIndexCount = 0;
	int selectedFolderID = -1;
	FolderNode* selectedFolder = nullptr;
public:
	static constexpr float fileButtonSize = 70.0f;
};