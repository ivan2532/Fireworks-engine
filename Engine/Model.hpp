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
struct GLFWwindow;

class Model : public Asset
{
public:
	static const std::vector<std::string> supportedFormats;
public:
	Model(unsigned id, const std::filesystem::path& name, const std::string&, Shader& shader) noexcept;
	void AddToScene(Scene&);
	void LoadCPU() noexcept;
	void LoadGPU() noexcept;
	void UnloadCPU() noexcept; //Sets the loaded flag to false (doesn't literally unload)
	void UnloadGPU() noexcept; //Sets the loaded flag to false (doesn't literally unload)
	void Draw() noexcept;
	void Draw(Shader& rShader) noexcept;
private:
	void ProcessNode(aiNode *node, const aiScene *scene, Transform* parent) noexcept;
	std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept;
public:
	void DrawInspector() noexcept override {} //TODO: Implement inspection of assets.
private:
	bool loadedCPU = false;
	bool loadedGPU = false;
	std::vector<GameObject> meshObjects;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::string directory; //For textures
	std::vector<Texture> loadedTextures;
	Shader& shader;
};