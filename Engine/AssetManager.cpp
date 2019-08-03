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
	mainWindow = editor.GetMainWindow()->GetWindow();

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	previewContext = glfwCreateWindow(1, 1, "Preview context", nullptr, nullptr);
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
	glfwMakeContextCurrent(nullptr);
	folders.clear();
	ScanDirectory(assetsDir, -1, std::this_thread::get_id());
}

unsigned AssetManager::GetPreviewFromMeta(const std::filesystem::path& metaPath, GLFWwindow* context) const noexcept
{
	if (context != nullptr)
	{
		glfwMakeContextCurrent(context);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glViewport(0, 0, 128, 128);
	}

	unsigned result;

	std::string data;
	data.reserve(128 * 128 * 3);
	std::ifstream metaStream(metaPath.string());

	std::getline(metaStream, data);
	std::getline(metaStream, data);
	std::getline(metaStream, data);

	if(data.length() > 15)
		data = data.substr(14, data.length() - 14);

	auto curContext = glfwGetCurrentContext();
	glGenTextures(1u, &result);
	//std::cout << glGetError() << std::endl;
	glBindTexture(GL_TEXTURE_2D, result);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (context != nullptr)
	{
		glfwMakeContextCurrent(nullptr);
	}

	return result;
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

	auto curID = std::this_thread::get_id();

	std::ostringstream metaPath;
	metaPath << (cacheDir / std::to_string(modelAsset->GetID())).string();
	metaPath << ".meta";

	bool locked = false;
	std::unique_lock guard(foldersMutex, std::defer_lock);

	bool metaReady = true;

	auto GenerateMeta = [&]()
	{
		std::ofstream metaStream(metaPath.str());
		metaStream << "path " << modelAsset->GetPath() << std::endl;
		metaStream << "type model" << std::endl;
		metaStream << "preview_image ";

		if (!locked)
		{
			locked = true;
			guard.lock();
		}

		if (curID != oldThreadID)
		{
			assetPreview.GeneratePreviewImage(metaStream, *modelAsset, previewContext);
			metaStream << std::endl;

			unsigned image = GetPreviewFromMeta(metaPath.str(), mainWindow);
			modelAsset->SetPreview(image);
		}
		else
		{
			assetPreview.GeneratePreviewImage(metaStream, *modelAsset, mainWindow);
			glfwMakeContextCurrent(mainWindow);
			metaStream << std::endl;

			unsigned image = GetPreviewFromMeta(metaPath.str(), mainWindow);
			modelAsset->SetPreview(image);
		}

		metaStream.close();
	};

	if (!fs::exists(metaPath.str()))
	{
		metaReady = false;
		GenerateMeta();
	}
	else
	{
		std::string testString;
		std::ifstream metaStream(metaPath.str());
		std::getline(metaStream, testString);
		std::getline(metaStream, testString);
		std::getline(metaStream, testString);

		if (testString.length() == 128 * 128 * 3 + 14)
			metaReady = true;
		else
			GenerateMeta();
	}

	if(metaReady)
	{
		if (!locked)
		{
			locked = true;
			guard.lock();
		}

		if (curID != oldThreadID)
		{
			unsigned image = GetPreviewFromMeta(metaPath.str(), mainWindow);
			modelAsset->SetPreview(image);
		}
		else
		{
			unsigned image = GetPreviewFromMeta(metaPath.str(), mainWindow);
			modelAsset->SetPreview(image);
		}
	}

	if (curID != oldThreadID)
	{
		if (!locked)
		{
			locked = true;
			guard.lock();
		}

		if(threadCount > 0)
			threadCount--;
	}

	folders[folderIndex].assets.push_back(std::move(modelAsset));

	if(locked)
		guard.unlock();
}
