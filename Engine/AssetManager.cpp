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
}

void AssetManager::ScanDirectory(const std::filesystem::path& directory, int parentIndex) noexcept
{
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		FolderNode newFolder;
		newFolder.path = directory.string();
		newFolder.name = directory.filename().string();
		newFolder.parentIndex = parentIndex;

		int currentIndex = folders.size();

		folders.push_back(std::move(newFolder));

		if (parentIndex != -1)
			folders[parentIndex].childrenIndices.push_back(currentIndex);

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
						LoadModelAsset(entry.path(), folders[currentIndex]);
			}
		}
	}
}

void AssetManager::LoadModelAsset(const std::filesystem::path& path, FolderNode& folder) noexcept
{
	auto modelAsset = std::make_unique<Model>(path.filename().string(), path.string(), shader);
	folder.assets.push_back(std::move(modelAsset));
	std::cout << "Loaded model: " << path.string() << " in " << folder.name << std::endl;
}
