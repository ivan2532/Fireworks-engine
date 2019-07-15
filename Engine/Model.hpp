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

#define STB_IMAGE_IMPLEMENTATION

class Scene;
class Shader;

class Model
{
public:
	Model(std::string path, Shader& shader) noexcept;
	void MoveToScene(Scene&);
private:
	void LoadMesh(std::string path) noexcept;
	void ProcessNode(aiNode *node, const aiScene *scene, Transform* parent) noexcept;
	std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept;
	unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
private:
	std::vector<std::unique_ptr<GameObject>> meshObjects;
	std::string directory; //For textures
	std::vector<Texture> loadedTextures;
	Shader& shader;
};

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
{
	glm::mat4 to;

	to[0][0] = (float)from->a1; to[0][1] = (float)from->b1;  to[0][2] = (float)from->c1; to[0][3] = (float)from->d1;
	to[1][0] = (float)from->a2; to[1][1] = (float)from->b2;  to[1][2] = (float)from->c2; to[1][3] = (float)from->d2;
	to[2][0] = (float)from->a3; to[2][1] = (float)from->b3;  to[2][2] = (float)from->c3; to[2][3] = (float)from->d3;
	to[3][0] = (float)from->a4; to[3][1] = (float)from->b4;  to[3][2] = (float)from->c4; to[3][3] = (float)from->d4;

	return to;
}