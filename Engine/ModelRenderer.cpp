#include "ModelRenderer.hpp"

ModelRenderer::ModelRenderer(std::string modelPath, Shader& aShader)
	:
	model(modelPath),
	shader(aShader)
{
}

void ModelRenderer::Update() noexcept
{
	model.Draw(shader);
}
