#pragma once
#include <memory>
#include <fstream>
#include "Shader.hpp"

class Model;
struct GLFWwindow;

class AssetPreview
{
public:
	void GeneratePreviewImage(std::ofstream& outputStream, Model& model) noexcept;
private:
	//Shader previewShader;
	std::unique_ptr<Shader> previewShader;
	unsigned previewBuffer;
	unsigned colorBuffer;
	unsigned depthBuffer;
};