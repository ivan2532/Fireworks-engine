#include "Model.hpp"
#include "glad/glad.h"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "ImageLoader.h"
#include <iostream>

const std::vector<std::string> Model::supportedFormats = {
	".fbx",
	".dae",
	".blend",
	".3ds",
	".obj",
	".mdl"
};

Model::Model(const std::string& name, const std::string& path, Shader& s) noexcept
	:
	Asset(name, 0u),
	shader(s)
{
	LoadMesh(path);
}

void Model::LoadMesh(const std::string& path) noexcept
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << std::endl << std::endl << "Failed to load ASSIMP SCENE! " << importer.GetErrorString() << std::endl << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene, nullptr);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, Transform* parent) noexcept
{
	auto nodeTransform = Math::AiMatrix4x4ToGlm(&(node->mTransformation));

	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 rotation(1.0f);

	Math::DecomposeTransform(nodeTransform, &position, &scale, &rotation);
	glm::vec3 eulerAngles = Math::EulerAnglesFromRotation(rotation);

	auto object = GameObject(node->mName.C_Str());
	object.AddComponent<Transform>();
	object.GetComponent<Transform>().value()->AddShaderToUpdate(&shader);
	object.GetComponent<Transform>().value()->SetPosition(position);
	object.GetComponent<Transform>().value()->SetEulerAngles(eulerAngles);
	object.GetComponent<Transform>().value()->SetScale(scale);
	object.GetComponent<Transform>().value()->SetParent(parent);

	if (node->mNumMeshes > 0)
	{
		object.AddComponent<MeshRenderer>(shader);

		for (unsigned i = 0; i < node->mNumMeshes; i++)
		{
			auto mesh = ProcessMesh(
				scene->mMeshes[node->mMeshes[i]],
				scene
			);

			object.GetComponent<MeshRenderer>().value()->AddMesh(std::move(mesh));
		}
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene, object.GetComponent<Transform>().value());

	meshObjects.push_back(std::move(object));
}

std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept
{
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;

	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.texCoords = glm::zero<glm::vec2>();

		vertices.push_back(std::move(vertex));
	}

	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType_DIFFUSE, Diffuse);
		if (diffuseTextures.size() > 0)
			textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

		std::vector<Texture> specularTextures = LoadMaterialTextures(material, aiTextureType_SPECULAR, Specular);
		if (specularTextures.size() > 0)
			textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

		std::vector<Texture> normalTextures = LoadMaterialTextures(material, aiTextureType_HEIGHT, Normal);
		if(normalTextures.size() > 0)
			textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
	}

	return std::make_unique<Mesh>(mesh->mName.C_Str(), vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) noexcept
{
	std::vector<Texture> textures;

	for (unsigned i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString path;
		material->GetTexture(type, i, &path);

		bool alreadyLoaded = false;

		for (unsigned j = 0; j < loadedTextures.size(); j++)
		{
			if (loadedTextures[j].path.c_str() == path.C_Str())
			{
				textures.push_back(loadedTextures[j]);
				alreadyLoaded = true;
				break;
			}
		}

		if (!alreadyLoaded)
		{
			Texture texture;
			texture.id = ImageLoader::TextureFromFile(path.C_Str(), directory);
			texture.path = path.C_Str();
			texture.type = typeName;

			textures.push_back(texture); //Have to invoke the copy constructor here
			loadedTextures.push_back(std::move(texture)); //Can use move semantics here
		}
	}

	return std::move(textures);
}

void Model::AddToScene(Scene& scene)
{
	for (auto& object : meshObjects)
		scene.AddSceneObject(object);
}

void Model::InitMeshes() noexcept
{
	for (auto& object : meshObjects)
	{
		auto meshRenderer = object.GetComponent<MeshRenderer>();

		if (!meshRenderer.has_value())
			continue;

		for (auto& mesh : meshRenderer.value()->GetMeshes())
		{
			mesh->InitMesh();
		}
	}
}
