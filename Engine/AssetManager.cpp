#include "OpenGLIncludes.hpp"
#include "AssetManager.hpp"
#include "Model.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <future>

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
	auto start = std::chrono::steady_clock::now();

	folders.clear();
	ScanDirectory(assetsDir, -1);

	for (auto& folder : folders)
	{
		for (auto& asset : folder.assets)
		{
			if (auto check = dynamic_cast<Model*>(asset.get()))
				check->InitMeshes();
		}
	}

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock::now() - start);
	std::cout << std::endl << duration.count() << std::endl;
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

		foldersMutex.lock();
		folders.push_back(std::move(newFolder));

		if (parentIndex != -1)
			folders[parentIndex].childrenIndices.push_back(currentIndex);
		foldersMutex.unlock();

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_directory(entry.status()))
			{
				//ScanDirectory(entry.path(), currentIndex);
				auto newThread = std::async(&AssetManager::ScanDirectory, this, std::ref(entry.path()), currentIndex);
			}
			else
			{
				std::string fileExtension = entry.path().extension().string();

				for (const auto& modelExtension : Model::supportedFormats)
				{
					if (modelExtension == fileExtension)
					{
						std::cout << "Started loading model!" << std::endl;
						//LoadModelAsset(entry.path(), folders[currentIndex]);
						auto newThread = std::async(&AssetManager::LoadModelAsset, this, std::ref(entry.path()), std::ref(folders[currentIndex]));
						continue;
					}
				}
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
