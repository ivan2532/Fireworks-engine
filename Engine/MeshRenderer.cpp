#include "MeshRenderer.hpp"
#include "imguiIncludes.hpp"
#include <sstream>

MeshRenderer::MeshRenderer(Shader& s)
	:
	shader(s)
{
}

void MeshRenderer::AddMesh(Mesh* m) noexcept
{
	meshes.push_back(m);
}

std::vector<Mesh*>& MeshRenderer::GetMeshes() noexcept
{
	return meshes;
}

void MeshRenderer::Update() noexcept
{
	for(auto& mesh : meshes)
		mesh->Draw(shader);
}

void MeshRenderer::DrawInspector(Editor& editor) noexcept
{
	if (ImGui::CollapsingHeader("Mesh renderer"))
	{
		for (auto& mesh : meshes)
		{
			std::ostringstream sstream;
			sstream << "Mesh name: " << mesh->GetName() << std::endl << "Faces: " << mesh->indices.size() / 3;
			ImGui::Text(sstream.str().c_str());
		}
	}
}
