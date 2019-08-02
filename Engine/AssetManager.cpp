#include "OpenGLIncludes.hpp"
#include "AssetManager.hpp"
#include "Model.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include <iostream>
#include <thread>
#include <future>

namespace fs = std::filesystem;

AssetManager::AssetManager(Editor& editor, const std::string& projectDirectory, Shader& s) noexcept
	:
	assetsDirString(projectDirectory),
	shader(s),
	hardwareThreads(std::thread::hardware_concurrency())
{
	assetsDir = fs::path(assetsDirString);

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

	//std::thread scanner(&AssetManager::ScanDirectory, this, assetsDir, -1);
	//scanner.detach();
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

					//std::cout << "FOLDER SCAN NEW THREAD, COUNT: " << threadCount << std::endl;
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
							//std::cout << "MODEL SCAN NEW THREAD, COUNT: " << threadCount << std::endl;
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
	//std::cout << "Starting model load. Thread count: " << threadCount << std::endl;
	auto modelAsset = std::make_unique<Model>(path.filename().string(), path.string(), shader);
	auto curID = std::this_thread::get_id();

	if (curID != oldThreadID)
	{
		std::lock_guard guard(foldersMutex);
		glfwMakeContextCurrent(scanningContext);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		modelAsset->InitMeshes();
		folders[folderIndex].assets.push_back(std::move(modelAsset));
		glfwMakeContextCurrent(nullptr);

		if(threadCount > 0)
			threadCount--;
	}
	else
	{
		modelAsset->InitMeshes();
		folders[folderIndex].assets.push_back(std::move(modelAsset));
	}

	//std::cout << "Ended model load. Thread count: " << threadCount << std::endl;
}
