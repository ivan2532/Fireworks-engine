#pragma once
#include <string>

class Asset
{
public:
	Asset(const std::string& name, unsigned image) noexcept;
public:
	virtual void DrawInspector() noexcept = 0;
	std::string GetName() const noexcept;
private:
	std::string assetName;
	unsigned imageID;
	unsigned index = -1;
};