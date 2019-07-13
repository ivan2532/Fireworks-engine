#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader
{
public:
	//Delete unnecessary constructors
	Shader() = delete;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(std::string vsPath, std::string fsPath);
public:
	void Use() noexcept;
	unsigned GetID() const noexcept;

	//Uniform setters
	void SetInt(const std::string& name, int value) noexcept;
	void SetFloat(const std::string& name, float value) noexcept;
	void SetDouble(const std::string& name, double value) noexcept;
	void SetBool(const std::string& name, bool value) noexcept;

	void SetVec3(const std::string& name, glm::vec3 value) noexcept;
	void SetVec3(const std::string& name, float x, float y, float z) noexcept;

	void SetVec4(const std::string& name, glm::vec4 value) noexcept;
	void SetVec4(const std::string& name, float x, float y, float z, float w) noexcept;

	void SetMat3x3(const std::string& name, glm::mat3x3 value) noexcept;
	void SetMat4x3(const std::string& name, glm::mat4x3 value) noexcept;
	void SetMat4x4(const std::string& name, glm::mat4x4 value) noexcept;
private:
	unsigned programID;
};