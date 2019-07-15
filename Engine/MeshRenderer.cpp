#include "MeshRenderer.hpp"

MeshRenderer::MeshRenderer(GameObject* go, Shader& s)
	:
	Component(go),
	shader(s)
{
}

void MeshRenderer::AddMesh(std::unique_ptr<Mesh> m) noexcept
{
	meshes.push_back(std::move(m));
}

void MeshRenderer::Update() noexcept
{
	for(auto& mesh : meshes)
		mesh->Draw(shader);
}
