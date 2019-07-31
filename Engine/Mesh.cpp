#include "OpenGLIncludes.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include <iostream>

Mesh::Mesh(const std::string& n, std::vector<Vertex> v, std::vector<unsigned> i, std::vector<Texture> t) noexcept
	:
	name(n),
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

std::string Mesh::GetName() const noexcept
{
	return name;
}

void Mesh::InitMesh() noexcept
{
	glGenVertexArrays(1u, &vertexArrayID);
	glGenBuffers(1u, &vertexBufferID);
	glGenBuffers(1u, &elementBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0u, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0u);

	glVertexAttribPointer(1u, 3u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1u);

	glVertexAttribPointer(2u, 2u, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(2u);

	glBindVertexArray(0);
}
