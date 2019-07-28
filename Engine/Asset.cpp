#include "Asset.hpp"

Asset::Asset(const std::string& name, unsigned image) noexcept
	:
	assetName(name),
	imageID(image)
{
}
