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
	void AddMesh(Mesh* m) noexcept;
	std::vector<Mesh*>& GetMeshes() noexcept;
public:
	void Update() noexcept override;
public:
	void DrawInspector(Editor& editor) noexcept override;
private:
	std::vector<Mesh*> meshes;
	Shader& shader;
};