#include "Mesh.hpp"
#include "Shader.hpp"
#include "glad/glad.h"
#include <iostream>

#define DETAILED_LOGGING

Mesh::Mesh(std::vector<Vertex> v, std::vector<unsigned> i, std::vector<Texture> t) noexcept
	:
	vertices(std::move(v)),
	indices(std::move(i)),
	textures(std::move(t))
{
	InitMesh();
}

Mesh::Mesh(const Mesh& rhs) noexcept
	:
	vertices(rhs.vertices),
	indices(rhs.indices),
	textures(rhs.textures),
	vertexBufferID(rhs.vertexBufferID),
	elementBufferID(rhs.elementBufferID),
	vertexArrayID(rhs.vertexArrayID)
{
	InitMesh();
}

Mesh& Mesh::operator=(const Mesh& rhs) noexcept
{
	vertices = rhs.vertices;
	indices = rhs.indices;
	textures = rhs.textures;
	vertexBufferID = rhs.vertexBufferID;
	elementBufferID = rhs.elementBufferID;
	vertexArrayID = rhs.vertexArrayID;

	InitMesh();

	return *this;
}

Mesh::Mesh(Mesh&& rhs) noexcept
	:
	vertices(std::move(rhs.vertices)),
	indices(std::move(rhs.indices)),
	textures(std::move(rhs.textures)),
	vertexBufferID(std::move(rhs.vertexBufferID)),
	elementBufferID(std::move(rhs.elementBufferID)),
	vertexArrayID(std::move(rhs.vertexArrayID))
{
	InitMesh();
}

Mesh& Mesh::operator=(Mesh&& rhs) noexcept
{
	vertices = std::move(rhs.vertices);
	indices = std::move(rhs.indices);
	textures = std::move(rhs.textures);
	vertexBufferID = std::move(rhs.vertexBufferID);
	elementBufferID = std::move(rhs.elementBufferID);
	vertexArrayID = std::move(rhs.vertexArrayID);

	InitMesh();

	return *this;
}

void Mesh::Draw(Shader& shader) const noexcept
{
	shader.Use();
	unsigned diffuseNum = 1, specularNum = 1, normalNum = 1, metallicNum = 1;

	for (unsigned i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

		std::string texName;
		switch (textures[i].type)
		{
			case Diffuse:
				texName = "tex_diffuse" + std::to_string(diffuseNum++);
				break;
			case Specular:
				texName = "tex_specular" + std::to_string(specularNum++);
				break;
			case Normal:
				texName = "tex_normal" + std::to_string(normalNum++);
				break;
			default:
				std::cout << "Unknow texture type!";
				break;
		}

		shader.SetInt(texName, i);
	}

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::InitMesh() noexcept
{
#ifdef DETAILED_LOGGING
	std::cout << std::endl << std::endl << "---STARTING MESH INITIALIZATION" << std::endl;
	std::cout << "Generating vertex array." << std::endl;
#endif
	glGenVertexArrays(1u, &vertexArrayID);

#ifdef DETAILED_LOGGING
	std::cout << "Generating vertex buffer." << std::endl;
#endif
	glGenBuffers(1u, &vertexBufferID);

#ifdef DETAILED_LOGGING
	std::cout << "Generating element buffer." << std::endl;
#endif
	glGenBuffers(1u, &elementBufferID);

#ifdef DETAILED_LOGGING
	std::cout << "Binding vertex array." << std::endl;
#endif
	glBindVertexArray(vertexArrayID);

#ifdef DETAILED_LOGGING
	std::cout << "Binding vertex buffer." << std::endl;
#endif
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

#ifdef DETAILED_LOGGING
	std::cout << "Setting vertex buffer data." << std::endl;
#endif
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

#ifdef DETAILED_LOGGING
	std::cout << "Binding element buffer." << std::endl;
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
#ifdef DETAILED_LOGGING
	std::cout << "Setting index buffer data." << std::endl;
#endif
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);

#ifdef DETAILED_LOGGING
	std::cout << "Setting vertex layout." << std::endl;
#endif
	glVertexAttribPointer(0u, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0u);

	glVertexAttribPointer(1u, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1u);

	glVertexAttribPointer(2u, 2u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2u);

#ifdef DETAILED_LOGGING
	std::cout << "Unbinding vertex array." << std::endl;
#endif
	glBindVertexArray(0);

#ifdef DETAILED_LOGGING
	std::cout << "Successfully loaded textures:" << std::endl;
	for (unsigned i = 0; i < textures.size(); i++)
		std::cout << "i: " << i << " | Type: " << textures[i].type << " | Path: " << textures[i].path << std::endl;
#endif

#ifdef DETAILED_LOGGING
	std::cout << "---ENDING MESH INITIALIZATION." << std::endl << std::endl;
#endif
}
