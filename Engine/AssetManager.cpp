#include "OpenGLIncludes.hpp"
#include "AssetManager.hpp"
#include "Model.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include <iostream>
#include <thread>
#include <future>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

AssetManager::AssetManager(Editor& editor, const std::string& projectDirectory, Shader& s) noexcept
	:
	assetsDirString(projectDirectory),
	shader(s),
	hardwareThreads(std::thread::hardware_concurrency())
{
	assetsDir = fs::path(assetsDirString);
	cacheDir = assetsDir.parent_path() / "Cache";

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	scanningContext = glfwCreateWindow(1, 1, "", nullptr, editor.GetMainWindow()->GetWindow());
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
	ScanDirectory(assetsDir, -1, std::this_thread::get_id());
}

void AssetManager::ScanDirectory(const std::filesystem::path& directory, int parentIndex, const std::thread::id& oldThreadID) noexcept
{
	auto curID = std::this_thread::get_id();

	if (fs::exists(directory) && fs::is_directory(directory))
	{
		FolderNode newFolder;
		newFolder.path = directory.string();
		newFolder.name = directory.filename().string();
		newFolder.parentIndex = parentIndex;
		int currentIndex;

		if (curID != oldThreadID)
		{
			std::lock_guard guard(foldersMutex);

			currentIndex = folders.size();
			folders.push_back(std::move(newFolder));

			if (parentIndex != -1)
				folders[parentIndex].childrenIndices.push_back(currentIndex);
		}
		else
		{
			currentIndex = folders.size();
			folders.push_back(std::move(newFolder));

			if (parentIndex != -1)
				folders[parentIndex].childrenIndices.push_back(currentIndex);
		}

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_directory(entry.status()))
			{
				if (threadCount < hardwareThreads)
				{
					threadCount++;

					std::thread newThread(&AssetManager::ScanDirectory, this,
						entry.path(), currentIndex, std::ref(oldThreadID));
					newThread.detach();
				}
				else
					ScanDirectory(entry.path(), currentIndex, oldThreadID);
			}
			else
			{
				std::string fileExtension = entry.path().extension().string();

				for (const auto& modelExtension : Model::supportedFormats)
				{
					if (modelExtension == fileExtension)
					{
						if (threadCount < hardwareThreads)
						{
							threadCount++;
							std::thread newThread(&AssetManager::LoadModelAsset, this,
								entry.path(), currentIndex, std::ref(oldThreadID));
							newThread.detach();
						}
						else
							LoadModelAsset(entry.path(), currentIndex, oldThreadID);

						break;
					}
				}
			}
		}
	}

	if (curID != oldThreadID)
	{
		if (threadCount > 0)
			threadCount--;
	}
}

void AssetManager::LoadModelAsset(const std::filesystem::path& path, int folderIndex, const std::thread::id& oldThreadID) noexcept
{
	auto modelAsset = std::make_unique<Model>(currentID++, path.string(), path.stem().string(), shader);

	std::ostringstream metaPath;
	metaPath << (cacheDir / std::to_string(modelAsset->GetID())).string();
	metaPath << ".meta";

	if (!fs::exists(metaPath.str()))
	{
		std::ofstream metaStream(metaPath.str());
		metaStream << "#fireworks_meta" << std::endl;
		metaStream << "path " << modelAsset->GetPath() << std::endl;
		metaStream << "type model" << std::endl;
		metaStream << "preview_image NULL" << std::endl;
		metaStream.close();
	}

	auto curID = std::this_thread::get_id();

	if (curID != oldThreadID)
	{
		std::lock_guard guard(foldersMutex);
		folders[folderIndex].assets.push_back(std::move(modelAsset));

		if(threadCount > 0)
			threadCount--;
	}
	else
	{
		folders[folderIndex].assets.push_back(std::move(modelAsset));
	}
}
