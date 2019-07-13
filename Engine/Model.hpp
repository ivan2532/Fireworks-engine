#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Mesh.hpp"
#include <string>
#include <vector>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION

class Shader;

class Model
{
public:
	Model(std::string path) noexcept;
	void Draw(Shader& shader) const noexcept;
private:
	void LoadMesh(std::string path) noexcept;
	void ProcessNode(aiNode *node, const aiScene *scene) noexcept;
	std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept;
	unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::string directory; //For textures
	std::vector<Texture> loadedTextures;
};