#include "Model.hpp"
#include "glad/glad.h"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Math.hpp"
#include "ImageLoader.h"
#include <iostream>

//#define DETAILED_LOGGING

Model::Model(std::string path, Shader& s) noexcept
	:
	shader(s)
{
	LoadMesh(path);
}

void Model::LoadMesh(std::string path) noexcept
{
#ifdef DETAILED_LOGGING
	std::cout << std::endl << std::endl << "------------------STARTING MODEL LOADING" << std::endl;
	std::cout << "Loading aiScene." << std::endl;
#endif
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << std::endl << std::endl << "Failed to load ASSIMP SCENE! " << importer.GetErrorString() << std::endl << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene, nullptr);

#ifdef DETAILED_LOGGING
	std::cout << "------------------ENDED MODEL INITIALIZATION" << std::endl;
#endif
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, Transform* parent) noexcept
{
#ifdef DETAILED_LOGGING
	std::cout << "---STARTING PROCESS NODE" << std::endl;
#endif
	auto nodeTransform = Math::AiMatrix4x4ToGlm(&(node->mTransformation));

	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 rotation(1.0f);

	Math::DecomposeTransform(nodeTransform, &position, &scale, &rotation);
	glm::vec3 eulerAngles = Math::EulerAnglesFromRotation(rotation);

	auto object = std::make_unique<GameObject>(node->mName.C_Str());
	object->AddComponent(std::make_unique<Transform>(object.get()));
	object->GetComponent<Transform>().value()->AddShaderToUpdate(std::make_unique<Shader>(shader));
	object->GetComponent<Transform>().value()->SetPosition(position);
	object->GetComponent<Transform>().value()->SetEulerAngles(eulerAngles);
	object->GetComponent<Transform>().value()->SetScale(scale);
	object->GetComponent<Transform>().value()->SetParent(parent);

	if (node->mNumMeshes > 0)
	{
		object->AddComponent(std::make_unique<MeshRenderer>(object.get(), shader));

		for (unsigned i = 0; i < node->mNumMeshes; i++)
		{
			auto mesh = ProcessMesh(
				scene->mMeshes[node->mMeshes[i]],
				scene
			);

			object->GetComponent<MeshRenderer>().value()->AddMesh(std::move(mesh));
		}
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene, object->GetComponent<Transform>().value());

	meshObjects.push_back(std::move(object));

#ifdef DETAILED_LOGGING
	std::cout << "---ENDED PROCESS NODE" << std::endl;
#endif
}

std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) noexcept
{
#ifdef DETAILED_LOGGING
	std::cout << "---STARTING PROCESS MESH" << std::endl;
#endif
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;

#ifdef DETAILED_LOGGING
	std::cout << "Processing mesh vertices." << std::endl;
#endif
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

#ifdef DETAILED_LOGGING
	std::cout << "Processing mesh indices." << std::endl;
#endif
	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

#ifdef DETAILED_LOGGING
	std::cout << "Processing mesh materials." << std::endl;
#endif
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
#ifdef DETAILED_LOGGING
	std::cout << "---STARTING LOAD MATERIAL TEXTURES" << std::endl;
#endif
	std::vector<Texture> textures;

	for (unsigned i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString path;
		material->GetTexture(type, i, &path);

		bool alreadyLoaded = false;

#ifdef DETAILED_LOGGING
		std::cout << "Checking pre loaded textures." << std::endl;
#endif
		for (unsigned j = 0; j < loadedTextures.size(); j++)
		{
			if (loadedTextures[j].path.c_str() == path.C_Str())
			{
				textures.push_back(loadedTextures[j]);
				alreadyLoaded = true;
#ifdef DETAILED_LOGGING
				std::cout << "Found pre loaded texture." << std::endl;
#endif
				break;
			}
		}

		if (!alreadyLoaded)
		{
#ifdef DETAILED_LOGGING
			std::cout << "No pre loaded texture, reading texture." << std::endl;
#endif
			Texture texture;
			texture.id = ImageLoader::TextureFromFile(path.C_Str(), directory);
			texture.path = path.C_Str();
			texture.type = typeName;

			textures.push_back(texture); //Have to invoke the copy constructor here
			loadedTextures.push_back(std::move(texture)); //Can use move semantics here
		}
	}

#ifdef DETAILED_LOGGING
	std::cout << "---ENDED LOAD MATERIAL TEXTURES" << std::endl;
#endif

	return std::move(textures);
}

void Model::MoveToScene(Scene& scene)
{
	for (auto& object : meshObjects)
		scene.MoveSceneObject(std::move(object));
}
