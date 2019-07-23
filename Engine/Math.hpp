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

	static inline float* Glm4x4ToArray(const glm::mat4* value)
	{
		float result[16];

		result[0]  = (float)(*value)[0][0]; result[1]  = (float)(*value)[0][1];  result[2]  = (float)(*value)[0][2]; result[3]  = (float)(*value)[0][3];
		result[4]  = (float)(*value)[1][0]; result[5]  = (float)(*value)[1][1];  result[6]  = (float)(*value)[1][2]; result[7]  = (float)(*value)[1][3];
		result[8]  = (float)(*value)[2][0]; result[9]  = (float)(*value)[2][1];  result[10] = (float)(*value)[2][2]; result[11] = (float)(*value)[2][3];
		result[12] = (float)(*value)[3][0]; result[13] = (float)(*value)[3][1];  result[14] = (float)(*value)[3][2]; result[15] = (float)(*value)[3][3];

		return result;
	}
};