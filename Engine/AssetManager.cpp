#include "AssetManager.hpp"

AssetManager::AssetManager(const std::string& projectDirectory) noexcept
	:
	projectDir(projectDirectory)
{
	ScanAssets();
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
