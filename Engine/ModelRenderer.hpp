#pragma once
#include "Component.hpp"
#include "Model.hpp"
#include <string>

class Shader;

class ModelRenderer : public Component
{
public:
	ModelRenderer(std::string modelPath, Shader& shader);
public:
	void Update() noexcept override;
private:
	Model model;
	Shader& shader;
};