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
	std::unique_ptr<Component> Clone(GameObject* go) const noexcept override;
	void Update() noexcept override;
public:
	void DrawInspector(Editor& editor) noexcept override;
private:
	std::vector<Mesh*> meshes;
	Shader& shader;
};