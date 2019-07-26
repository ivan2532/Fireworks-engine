#pragma once
#include "Mesh.hpp"
#include "assimpIncludes.hpp"
#include "glmIncludes.hpp"
#include "Transform.hpp"
#include "MeshRenderer.hpp"
#include "GameObject.hpp"
#include <string>
#include <vector>
#include <memory>

class Scene;
class Shader;

class Model
{
public:
	Model(std::string path, Shader& shader) noexcept;
	void AddToScene(Scene&);
private:
	void LoadMesh(std::string path) noexcept;
	void ProcessNode(aiNode *node, const aiScene *scene, Transform* parent) noexcept;
	std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept;
private:
	std::vector<GameObject> meshObjects;
	std::string directory; //For textures
	std::vector<Texture> loadedTextures;
	Shader& shader;
};