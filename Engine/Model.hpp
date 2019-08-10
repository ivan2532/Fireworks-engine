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
	void LoadCPU(bool loadPreview = false) noexcept;
	void LoadGPU() noexcept;
	void UnloadCPU() noexcept; //Sets the loaded flag to false (doesn't literally unload)
	void UnloadGPU() noexcept; //Sets the loaded flag to false (doesn't literally unload)
	void Draw() noexcept;
	void Draw(Shader& rShader) noexcept;
	GameObject& GetObject() noexcept;
public: //Preview getters
	glm::vec3 GetSphereCenter() const noexcept;
	float GetSphereRadius() const noexcept;
private:
	void LoadGPURecurse(GameObject& object) noexcept;
	void ProcessNode(aiNode *node, const aiScene *scene, Transform* parent) noexcept;
	std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept;
public:
	void DrawInspector() noexcept override {} //TODO: Implement inspection of assets.
private:
	bool loadedCPU = false;
	bool loadedGPU = false;
	bool loadPreviewValues = false;
	GameObject meshObject;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::string directory; //For textures
	std::vector<Texture> loadedTextures;
	Shader& shader;
	glm::vec3 sphereCenter = glm::zero<glm::vec3>();
	float sphereRadius = 0.0f;
};