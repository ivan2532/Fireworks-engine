#pragma once
#include <string>
#include <random>
#include <filesystem>

class Asset
{
public:
	Asset(unsigned id, const std::filesystem::path& path, const std::string& name, unsigned image) noexcept;
public:
	virtual void DrawInspector() noexcept = 0;
	std::filesystem::path GetPath() const noexcept;
	std::string GetName() const noexcept;
	unsigned GetID() const noexcept;
	void SetPreview(unsigned value) noexcept;
	unsigned GetPreview() const noexcept;
protected:
	std::filesystem::path assetPath;
	std::string assetName;
	unsigned assetID;
	unsigned imageID;
	unsigned index = -1;
};