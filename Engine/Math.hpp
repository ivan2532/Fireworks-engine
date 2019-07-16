#pragma once
#include "assimpIncludes.hpp"
#include "glmIncludes.hpp"

class Math
{
public:
	static void DecomposeTransform(glm::mat4 trans, glm::vec3* translation, glm::vec3* scale, glm::mat4* rotation);
	static glm::vec3 EulerAnglesFromRotation(const glm::mat4& rotation);
	static inline glm::mat4 AiMatrix4x4ToGlm(const aiMatrix4x4* value)
	{
		glm::mat4 result;

		result[0][0] = (float)value->a1; result[0][1] = (float)value->b1;  result[0][2] = (float)value->c1; result[0][3] = (float)value->d1;
		result[1][0] = (float)value->a2; result[1][1] = (float)value->b2;  result[1][2] = (float)value->c2; result[1][3] = (float)value->d2;
		result[2][0] = (float)value->a3; result[2][1] = (float)value->b3;  result[2][2] = (float)value->c3; result[2][3] = (float)value->d3;
		result[3][0] = (float)value->a4; result[3][1] = (float)value->b4;  result[3][2] = (float)value->c4; result[3][3] = (float)value->d4;

		return result;
	}
};