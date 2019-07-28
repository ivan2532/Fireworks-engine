#include "AssetManager.hpp"

namespace fs = std::filesystem;

AssetManager::AssetManager(const std::string& projectDirectory) noexcept
	:
	assetsDirString(projectDirectory)
{
	assetsDir = fs::path(assetsDirString);
	ScanAssets();
}

std::string AssetManager::GetAssetsDirString() const noexcept
{
	return assetsDirString;
}

std::filesystem::path AssetManager::GetAssetsDir() const noexcept
{
	return assetsDir;
}

void AssetManager::ScanAssets() noexcept
{
	//Iterate through project directory.
}

void AssetManager::ImportAsset(const std::string& path) noexcept
{
	//Create asset from file and push into the assets vector.
}

void AssetManager::DeleteAsset(unsigned index) noexcept
{
	//Delete file and erase from assets vector.
}
