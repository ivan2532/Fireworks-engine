#pragma once
#include "Asset.hpp"
#include "AssetPreview.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <mutex>
#include <atomic>

struct FolderNode
{
	std::string path;
	std::string name;
	bool expanded = false;
	int parentIndex = -1;
	std::vector<int> childrenIndices;
	std::vector<std::unique_ptr<Asset>> assets;
};

class Shader;
class Editor;

struct GLFWwindow;

class AssetManager
{
	friend class AssetExplorerWindow;
public:
	AssetManager(Editor& editor, const std::string& projectDirectory, Shader& shader) noexcept;
public:
	std::string GetAssetsDirString() const noexcept;
	std::filesystem::path GetAssetsDir() const noexcept;
public:
	void ScanAssets() noexcept;
	unsigned GetPreviewFromMeta(const std::filesystem::path& metaPath) const noexcept;
private:
	void ScanDirectory(const std::filesystem::path& directory, int parentIndex) noexcept;
	void LoadModelAsset(const std::filesystem::path& path, int folderIndex) noexcept;
private:
	AssetPreview assetPreview;
	std::string assetsDirString;
	std::filesystem::path assetsDir;
	std::filesystem::path cacheDir;
	std::vector<FolderNode> folders;
	Shader& shader;
	unsigned currentID = 0u;
};