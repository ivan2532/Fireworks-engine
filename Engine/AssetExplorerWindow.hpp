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
	void DrawFolderContents(const FolderNode& folder) const noexcept;
private:
	AssetManager assetManager;
	int nodeIndexCount = 0;
	int selectedFolderID = -1;
	FolderNode* selectedFolder = nullptr;
public:
	static constexpr float fileButtonSize = 70.0f;
};