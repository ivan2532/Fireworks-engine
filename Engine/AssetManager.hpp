#pragma once
#include <string>
#include <vector>

class Asset;

class AssetManager
{
public:
	AssetManager(const std::string& projectDirectory) noexcept;
public:
	void ScanAssets() noexcept;
	void ImportAsset(const std::string& path) noexcept;
	void DeleteAsset(unsigned index) noexcept;
private:
	std::string projectDir;
	std::vector<std::string> assets;
};