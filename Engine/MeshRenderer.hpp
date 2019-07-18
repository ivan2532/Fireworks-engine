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
	MeshRenderer(GameObject* go, Shader& shader);
	void AddMesh(std::unique_ptr<Mesh> m) noexcept;
public:
	void Update() noexcept override;
private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	Shader& shader;
};