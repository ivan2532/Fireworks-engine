#include "OpenGLIncludes.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include <iostream>
#include <thread>

Mesh::Mesh(
	const std::string& n,
	const std::vector<Vertex>& v,
	const std::vector<unsigned>& i,
	const std::vector<Texture>& t,
	const glm::vec3& center,
	float radius
) noexcept
	:
	name(n),
	vertices(v),
	indices(i),
	textures(t),
	sphereCenter(center),
	sphereRadius(radius)
{
}

void Mesh::Draw(Shader& shader) const noexcept
{
	if (!loadedGPU)
		std::cout << "WARNING: Drawing mesh while it isn't loaded!" << std::endl;

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
			case Ambient_Occlusion:
				texName = "tex_ao" + std::to_string(normalNum++);
				break;
			case Metalness:
				texName = "tex_metalness" + std::to_string(normalNum++);
				break;
			default:
				std::cout << "Unknow texture type!";
				break;
		}

		shader.SetInt(texName, i);
	}

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

std::string Mesh::GetName() const noexcept
{
	return name;
}

void Mesh::LoadGPU() noexcept
{
	if (loadedGPU)
		return;

	loadedGPU = true;

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

glm::vec3 Mesh::GetSphereCenter() const noexcept
{
	return sphereCenter;
}

float Mesh::GetSphereRadius() const noexcept
{
	return sphereRadius;
}
