#pragma once
#include "Asset.hpp"
#include <string>
#include <vector>
#include <filesystem>
#include <memory>

struct FolderNode
{
	std::string path;
	std::string name;
	int parentIndex = -1;
	std::vector<int> childrenIndices;
	bool expanded = false;
};

class Shader;

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
	void ImportAsset(const std::string& path) noexcept;
	void DeleteAsset(unsigned index) noexcept;
private:
	void ScanDirectory(const std::filesystem::path& directory, int parent) noexcept;
	void LoadModelAsset(const std::filesystem::path& path) noexcept;
private:
	std::string assetsDirString;
	std::filesystem::path assetsDir;
	std::vector<FolderNode> folders;
	std::vector<std::unique_ptr<Asset>> assets;
	Shader& shader;
};