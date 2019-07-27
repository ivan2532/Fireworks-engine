#include "MeshRenderer.hpp"
#include "imguiIncludes.hpp"
#include <sstream>

MeshRenderer::MeshRenderer(Shader& s)
	:
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
