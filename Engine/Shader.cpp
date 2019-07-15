#include "Shader.hpp"
#include "glad/glad.h"
#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(const Shader& rhs) noexcept
	:
	programID(rhs.programID)
{
}

Shader& Shader::operator=(const Shader& rhs) noexcept
{
	programID = rhs.programID;
	return *this;
}

Shader::Shader(Shader&& rhs) noexcept
	:
	programID(std::move(rhs.programID))
{
}

Shader& Shader::operator=(Shader&& rhs) noexcept
{
	programID = std::move(programID);
	return *this;
}

Shader::Shader(std::string vsPath, std::string fsPath)
{
	std::string vsSourceStr, fsSourceStr;
	std::stringstream vsStream, fsStream;

	std::ifstream vsFile, fsFile;
	vsFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fsFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		vsFile.open(vsPath);
		vsStream << vsFile.rdbuf();
		vsFile.close();

		fsFile.open(fsPath);
		fsStream << fsFile.rdbuf();
		fsFile.close();
		
		vsSourceStr = vsStream.str();
		fsSourceStr = fsStream.str();
	}
	catch (std::fstream::failure e)
	{
		std::cout << std::endl << std::endl << "Failed to load shaders! " << e.what() << std::endl << std::endl;
	}
	catch (...)
	{
		std::cout << std::endl << std::endl << "Unknown error while loading shaders!" << std::endl << std::endl;
	}

	const char* vsSource = vsSourceStr.c_str();
	const char* fsSource = fsSourceStr.c_str();

	unsigned vertexShaderID;
	unsigned fragmentShaderID;

	int success;
	char infoBuffer[512];

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1u, &vsSource, nullptr);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, sizeof(infoBuffer), nullptr, infoBuffer);
		std::cout << std::endl << std::endl << "Failed to compile VERTEX SHADER. " << infoBuffer << std::endl << std::endl;
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1u, &fsSource, nullptr);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, sizeof(infoBuffer), nullptr, infoBuffer);
		std::cout << std::endl << std::endl << "Failed to compile FRAGMENT SHADER. " << infoBuffer << std::endl << std::endl;
	}

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(programID, sizeof(infoBuffer), nullptr, infoBuffer);
		std::cout << std::endl << std::endl << "Failed to link program. " << infoBuffer << std::endl << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::Use() noexcept
{
	glUseProgram(programID);
}

unsigned Shader::GetID() const noexcept
{
	return programID;
}

void Shader::SetInt(const std::string& name, int value) noexcept
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) noexcept
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetDouble(const std::string& name, double value) noexcept
{
	glUniform1d(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetBool(const std::string& name, bool value) noexcept
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) noexcept
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1u, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) noexcept
{
	glm::vec3 value(x, y, z);
	SetVec3(name, value);
}

void Shader::SetVec4(const std::string& name, glm::vec4 value) noexcept
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1u, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) noexcept
{
	glm::vec4 value(x, y, z, w);
	SetVec4(name, value);
}

void Shader::SetMat3x3(const std::string& name, glm::mat3x3 value) noexcept
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1u, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4x3(const std::string& name, glm::mat4x3 value) noexcept
{
	glUniformMatrix4x3fv(glGetUniformLocation(programID, name.c_str()), 1u, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4x4(const std::string& name, glm::mat4x4 value) noexcept
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1u, GL_FALSE, glm::value_ptr(value));
}
