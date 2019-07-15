#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vector>

class Shader;

enum TextureType
{
	Diffuse,
	Specular,
	Normal
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned id;
	TextureType type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures) noexcept;
	~Mesh() = default;
	Mesh(const Mesh&) noexcept;
	Mesh& operator=(const Mesh&) noexcept;
	Mesh(Mesh&&) noexcept;
	Mesh& operator=(Mesh&&) noexcept;
	void Draw(Shader& shader) const noexcept;
private:
	void InitMesh() noexcept;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;
private:
	unsigned vertexBufferID;
	unsigned elementBufferID;
	unsigned vertexArrayID;
};