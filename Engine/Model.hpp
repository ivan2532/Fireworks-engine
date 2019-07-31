#pragma once
#include "Mesh.hpp"
#include "assimpIncludes.hpp"
#include "glmIncludes.hpp"
#include "Transform.hpp"
#include "MeshRenderer.hpp"
#include "GameObject.hpp"
#include "Asset.hpp"
#include <string>
#include <vector>
#include <memory>

class Scene;
class Shader;

class Model : public Asset
{
public:
	static const std::vector<std::string> supportedFormats;
public:
	Model(const std::string& name, const std::string&, Shader& shader) noexcept;
	void AddToScene(Scene&);
	void InitMeshes() noexcept;
private:
	void LoadMesh(const std::string& path) noexcept;
	void ProcessNode(aiNode *node, const aiScene *scene, Transform* parent) noexcept;
	std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept;
public:
	void DrawInspector() noexcept override {} //TODO: Implement inspection of assets.
private:
	std::vector<GameObject> meshObjects;
	std::string directory; //For textures
	std::vector<Texture> loadedTextures;
	Shader& shader;
};