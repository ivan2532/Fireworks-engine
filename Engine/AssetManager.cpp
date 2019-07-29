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
	folders.clear();
	ScanDirectory(assetsDir, -1);

	std::sort(
		folders.begin(),
		folders.end(),
		[](const Folder& lhs, const Folder& rhs)
		{
			return lhs.path < rhs.path;
		}
	);
}

void AssetManager::ImportAsset(const std::string& path) noexcept
{
	//Create asset from file and push into the assets vector.
}

void AssetManager::DeleteAsset(unsigned index) noexcept
{
	//Delete file and erase from assets vector.
}

void AssetManager::ScanDirectory(const std::filesystem::path& directory, int parent) noexcept
{
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		Folder newFolder;
		newFolder.path = directory.string();
		newFolder.name = directory.filename().string();
		newFolder.parentIndex = parent;

		int currentIndex = folders.size();

		folders.push_back(newFolder);

		if(parent != -1)
			folders[parent].childrenIndices.push_back(currentIndex);

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_directory(entry.status()))
			{
				ScanDirectory(entry.path(), currentIndex);
			}
		}
	}
}
