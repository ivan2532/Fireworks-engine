#pragma once
#include "Component.hpp"
#include "Model.hpp"
#include <string>

class GameObject;
class Shader;

class ModelRenderer : public Component
{
public:
	ModelRenderer(GameObject* go, std::string modelPath, Shader& shader);
public:
	void Update() noexcept override;
private:
	Model model;
	Shader& shader;
};