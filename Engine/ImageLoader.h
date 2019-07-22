#pragma once
#include <string>

class ImageLoader
{
public:
	static unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	static unsigned char* ImageDataFromFile(const char* path, int* width, int* height, int* numChannels);
};