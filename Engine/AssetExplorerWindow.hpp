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
	void DrawFolderTree(FolderNode& folder) noexcept;
private:
	AssetManager assetManager;
	int nodeIndexCount = 0;
	int selectedFolderID = -1;
	std::filesystem::path selectedFolder;
};