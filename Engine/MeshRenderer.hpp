#pragma once
#include "Component.hpp"
#include "Mesh.hpp"
#include <string>
#include <vector>
#include <memory>

class GameObject;
class Shader;

class MeshRenderer : public Component
{
public:
	MeshRenderer(Shader& shader);
	void AddMesh(std::unique_ptr<Mesh> m) noexcept;
	std::vector<std::unique_ptr<Mesh>>& GetMeshes() noexcept;
public:
	void Update() noexcept override;
public:
	void DrawInspector(Editor& editor) noexcept override;
private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	Shader& shader;
};