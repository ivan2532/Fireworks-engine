#pragma once
#include <string>
#include <vector>
#include <filesystem>

class Asset;

class AssetManager
{
public:
	AssetManager(const std::string& projectDirectory) noexcept;
public:
	std::string GetAssetsDirString() const noexcept;
	std::filesystem::path GetAssetsDir() const noexcept;
public:
	void ScanAssets() noexcept;
	void ImportAsset(const std::string& path) noexcept;
	void DeleteAsset(unsigned index) noexcept;
private:
	std::string assetsDirString;
	std::filesystem::path assetsDir;
	std::vector<std::string> assets;
};