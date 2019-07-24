#pragma once
#include "assimpIncludes.hpp"
#include "glmIncludes.hpp"

struct Float4x4
{
	float data[16];
};

class Math
{
public:
	static void DecomposeTransform(const glm::mat4& trans, glm::vec3* translation, glm::vec3* scale, glm::mat4* rotation) noexcept;
	static glm::mat4 AssembleTransform(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation) noexcept;

	static glm::vec3 EulerAnglesFromRotation(const glm::mat4& rotation) noexcept;

	static inline glm::mat4 AiMatrix4x4ToGlm(const aiMatrix4x4* value) noexcept
	{
		glm::mat4 result;

		result[0][0] = (float)value->a1; result[0][1] = (float)value->b1;  result[0][2] = (float)value->c1; result[0][3] = (float)value->d1;
		result[1][0] = (float)value->a2; result[1][1] = (float)value->b2;  result[1][2] = (float)value->c2; result[1][3] = (float)value->d2;
		result[2][0] = (float)value->a3; result[2][1] = (float)value->b3;  result[2][2] = (float)value->c3; result[2][3] = (float)value->d3;
		result[3][0] = (float)value->a4; result[3][1] = (float)value->b4;  result[3][2] = (float)value->c4; result[3][3] = (float)value->d4;

		return result;
	}

	static inline Float4x4 Glm4x4ToArray(const glm::mat4& value) noexcept
	{
		Float4x4 result;
		
		result.data[0]  = value[0][0]; result.data[1]  = value[0][1];  result.data[2]  = value[0][2]; result.data[3]  = value[0][3];
		result.data[4]  = value[1][0]; result.data[5]  = value[1][1];  result.data[6]  = value[1][2]; result.data[7]  = value[1][3];
		result.data[8]  = value[2][0]; result.data[9]  = value[2][1];  result.data[10] = value[2][2]; result.data[11] = value[2][3];
		result.data[12] = value[3][0]; result.data[13] = value[3][1];  result.data[14] = value[3][2]; result.data[15] = value[3][3];

		return result;
	}

	static inline glm::mat4 ArrayToGlm4x4(float* value) noexcept
	{
		glm::mat4 result;

		result[0][0] = value[0]; result[0][1]  = value[1];  result[0][2] = value[2];  result[0][3] = value[3];
		result[1][0] = value[4]; result[1][1]  = value[5];  result[1][2] = value[6];  result[1][3] = value[7];
		result[2][0] = value[8]; result[2][1]  = value[9];  result[2][2] = value[10]; result[2][3] = value[11];
		result[3][0] = value[12]; result[3][1] = value[13]; result[3][2] = value[14]; result[3][3] = value[15];

		return result;
	}
};