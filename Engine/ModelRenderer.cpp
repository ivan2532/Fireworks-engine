#include "ModelRenderer.hpp"
#include "GameObject.hpp"
#include <iostream>

ModelRenderer::ModelRenderer(GameObject* go, std::string modelPath, Shader& aShader)
	:
	Component(go),
	model(modelPath, aShader),
	shader(aShader)
{
}

void ModelRenderer::Update() noexcept
{
	//model.Draw(shader);
}
