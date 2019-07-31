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
	glm::vec3 pos = glm::zero<glm::vec3>();
	glm::vec3 normal = glm::zero<glm::vec3>();
	glm::vec2 texCoords = glm::zero<glm::vec2>();
};

struct Texture
{
	unsigned id = 0;
	TextureType type = Diffuse;
	std::string path = "";
};

class Mesh
{
public:
	Mesh(const std::string& name, std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures) noexcept;
	~Mesh() = default;
	Mesh(const Mesh&) noexcept;
	Mesh& operator=(const Mesh&) noexcept;
	Mesh(Mesh&&) noexcept;
	Mesh& operator=(Mesh&&) noexcept;
	void Draw(Shader& shader) const noexcept;
	std::string GetName() const noexcept;
	void InitMesh() noexcept;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;
private:
	std::string name;
	unsigned vertexBufferID;
	unsigned elementBufferID;
	unsigned vertexArrayID;
};