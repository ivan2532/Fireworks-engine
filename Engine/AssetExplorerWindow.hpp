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
	void DrawFolderTree(int folderIndex) noexcept;
	void DrawFolderContents(int folderIndex) const noexcept;
private:
	AssetManager assetManager;
	float folderWidth;
	float fileWidth;
	int nodeIndexCount = 0;
	int selectedFolderID = -1;
	int selectedFolderIndex = -1;
public:
	static constexpr float fileButtonSize = 70.0f;
};