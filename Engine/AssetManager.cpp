#include "AssetManager.hpp"
#include "Model.hpp"
#include <iostream>

namespace fs = std::filesystem;

AssetManager::AssetManager(const std::string& projectDirectory, Shader& s) noexcept
	:
	assetsDirString(projectDirectory),
	shader(s)
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
		[](const FolderNode& lhs, const FolderNode& rhs)
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
		FolderNode newFolder;
		newFolder.path = directory.string();
		newFolder.name = directory.filename().string();
		newFolder.parentIndex = parent;

		int currentIndex = folders.size();

		folders.push_back(newFolder);

		if (parent != -1)
			folders[parent].childrenIndices.push_back(currentIndex);

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_directory(entry.status()))
			{
				ScanDirectory(entry.path(), currentIndex);
			}
			else
			{
				std::string fileExtension = entry.path().extension().string();

				for (const auto& modelExtension : Model::supportedFormats)
					if (modelExtension == fileExtension)
						LoadModelAsset(entry.path());
			}
		}
	}
}

void AssetManager::LoadModelAsset(const std::filesystem::path& path) noexcept
{
	auto modelAsset = std::make_unique<Model>(path.string(), shader);
	assets.push_back(std::move(modelAsset));
	std::cout << "Loaded model: " << path.string() << std::endl;
}