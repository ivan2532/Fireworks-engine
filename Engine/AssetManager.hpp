#pragma once
#include "Asset.hpp"
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
private:
	void ScanDirectory(const std::filesystem::path& directory, int parentIndex, const std::thread::id& oldThreadID) noexcept;
	void LoadModelAsset(const std::filesystem::path& path, int folderIndex, const std::thread::id& oldThreadID) noexcept;
private:
	std::string assetsDirString;
	std::filesystem::path assetsDir;
	std::vector<FolderNode> folders;
	Shader& shader;
	std::mutex foldersMutex;
	GLFWwindow* scanningContext;
	std::atomic<unsigned> threadCount = 1u;
	const unsigned hardwareThreads;
};