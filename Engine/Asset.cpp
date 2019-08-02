#include "Asset.hpp"

Asset::Asset(unsigned id, const std::string& path, const std::string& name, unsigned image) noexcept
	:
	assetPath(path),
	assetName(name),
	assetID(id),
	imageID(image)
{
}

std::string Asset::GetPath() const noexcept
{
	return assetPath;
}

std::string Asset::GetName() const noexcept
{
	return assetName;
}

unsigned Asset::GetID() const noexcept
{
	return assetID;
}
