#include "Asset.hpp"

Asset::Asset(unsigned id, const std::filesystem::path& path, const std::string& name, unsigned image) noexcept
	:
	assetPath(path),
	assetName(name),
	assetID(id),
	imageID(image)
{
}

std::filesystem::path Asset::GetPath() const noexcept
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

unsigned Asset::GetPreview() const noexcept
{
	return imageID;
}

void Asset::SetPreview(unsigned value) noexcept
{
	imageID = value;
}
