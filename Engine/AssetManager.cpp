#include "OpenGLIncludes.hpp"
#include "AssetManager.hpp"
#include "Model.hpp"
#include "Editor.hpp"
#include "Engine.hpp"
#include <iostream>
#include <future>
#include <sstream>
#include <fstream>

AssetManager::AssetManager(Editor& editor, const std::string& projectDirectory, Shader& s) noexcept
	:
	assetsDirString(projectDirectory),
	shader(s)
{
	assetsDir = std::filesystem::path(assetsDirString);
	cacheDir = assetsDir.parent_path() / "Cache";

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

unsigned AssetManager::GetPreviewFromMeta(const std::filesystem::path& metaPath) const noexcept
{
	unsigned result;

	std::string data;
	data.reserve(128 * 128 * 3);
	std::ifstream metaStream(metaPath.string());

	std::getline(metaStream, data);
	std::getline(metaStream, data);
	std::getline(metaStream, data);

	if(data.length() > 15)
		data = data.substr(14, data.length() - 14);

	glGenTextures(1u, &result);

	glBindTexture(GL_TEXTURE_2D, result);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return result;
}

void AssetManager::ScanDirectory(const std::filesystem::path& directory, int parentIndex) noexcept
{
	auto curID = std::this_thread::get_id();

	if (std::filesystem::exists(directory) && std::filesystem::is_directory(directory))
	{
		FolderNode newFolder;
		newFolder.path = directory.string();
		newFolder.name = directory.filename().string();
		newFolder.parentIndex = parentIndex;
		int currentIndex;

		currentIndex = folders.size();
		folders.push_back(std::move(newFolder));

		if (parentIndex != -1)
			folders[parentIndex].childrenIndices.push_back(currentIndex);

		for (const auto& entry : std::filesystem::directory_iterator(directory))
		{
			if (std::filesystem::is_directory(entry.status()))
				ScanDirectory(entry.path(), currentIndex);
			else
			{
				std::string fileExtension = entry.path().extension().string();

				for (const auto& modelExtension : Model::supportedFormats)
				{
					if (modelExtension == fileExtension)
					{
						LoadModelAsset(entry.path(), currentIndex);
						break;
					}
				}
			}
		}
	}
}

void AssetManager::LoadModelAsset(const std::filesystem::path& path, int folderIndex) noexcept
{
	auto modelAsset = std::make_unique<Model>(currentID++, path.string(), path.stem().string(), shader);

	auto curID = std::this_thread::get_id();

	std::ostringstream metaPath;
	metaPath << (cacheDir / std::to_string(modelAsset->GetID())).string();
	metaPath << ".meta";

	bool metaReady = true;

	auto GenerateMeta = [&]()
	{
		std::cout << "Importing " << path.string() << std::endl;

		std::ofstream metaStream(metaPath.str());
		metaStream << "path " << modelAsset->GetPath() << std::endl;
		metaStream << "type model" << std::endl;
		metaStream << "preview_image ";

		assetPreview.GeneratePreviewImage(metaStream, *modelAsset);
		metaStream << std::endl;

		unsigned image = GetPreviewFromMeta(metaPath.str());
		modelAsset->SetPreview(image);

		metaStream.close();
	};

	if (!std::filesystem::exists(metaPath.str()))
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
		unsigned image = GetPreviewFromMeta(metaPath.str());
		modelAsset->SetPreview(image);
	}

	folders[folderIndex].assets.push_back(std::move(modelAsset));
}
