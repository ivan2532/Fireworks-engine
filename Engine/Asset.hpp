#pragma once
#include <string>
#include <random>


class Asset
{
public:
	Asset(unsigned id, const std::string& path, const std::string& name, unsigned image) noexcept;
public:
	virtual void DrawInspector() noexcept = 0;
	std::string GetPath() const noexcept;
	std::string GetName() const noexcept;
	unsigned GetID() const noexcept;
protected:
	std::string assetPath;
	std::string assetName;
	unsigned assetID;
	unsigned imageID;
	unsigned index = -1;
};