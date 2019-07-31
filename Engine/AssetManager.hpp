#pragma once
#include "Asset.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <mutex>

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

class AssetManager
{
	friend class AssetExplorerWindow;
public:
	AssetManager(const std::string& projectDirectory, Shader& shader) noexcept;
public:
	std::string GetAssetsDirString() const noexcept;
	std::filesystem::path GetAssetsDir() const noexcept;
public:
	void ScanAssets() noexcept;
private:
	void ScanDirectory(const std::filesystem::path& directory, int parentIndex) noexcept;
	void LoadModelAsset(const std::filesystem::path& path, FolderNode& folder) noexcept;
private:
	std::string assetsDirString;
	std::filesystem::path assetsDir;
	std::vector<FolderNode> folders;
	Shader& shader;
	std::mutex foldersMutex;
};