#pragma once
#include <string>

class Asset
{
public:
	Asset(const std::string& name, unsigned image) noexcept;
public:
	virtual void DrawInspector() = 0;
private:
	std::string assetName;
	unsigned imageID;
	unsigned index = -1;
};