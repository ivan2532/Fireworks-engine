#include "Math.hpp"
#include <iostream>

void Math::DecomposeTransform(const glm::mat4& trans, glm::vec3* translation, glm::vec3* scale, glm::mat4* rotation) noexcept
{
	translation->x = trans[3][0];
	translation->y = trans[3][1];
	translation->z = trans[3][2];

	scale->x = glm::length(glm::vec3(trans[0][0], trans[1][0], trans[2][0]));
	scale->y = glm::length(glm::vec3(trans[0][1], trans[1][1], trans[2][1]));
	scale->z = glm::length(glm::vec3(trans[0][2], trans[1][2], trans[2][2]));

	(*rotation)[0][0] = trans[0][0] / scale->x;
	(*rotation)[1][0] = trans[1][0] / scale->x;
	(*rotation)[2][0] = trans[2][0] / scale->x;
	(*rotation)[0][1] = trans[0][1] / scale->y;
	(*rotation)[1][1] = trans[1][1] / scale->y;
	(*rotation)[2][1] = trans[2][1] / scale->y;
	(*rotation)[0][2] = trans[0][2] / scale->z;
	(*rotation)[1][2] = trans[1][2] / scale->z;
	(*rotation)[2][2] = trans[2][2] / scale->z;
}

glm::mat4 Math::AssembleTransform(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation) noexcept
{
	glm::mat4 result(1.0f);

	result = glm::translate(result, translation);
	result = glm::scale(result, scale);

	result = glm::rotate(result, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // X, Y, Z order (column-major)
	result = glm::rotate(result, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	result = glm::rotate(result, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	return result;
}

glm::vec3 Math::EulerAnglesFromRotation(const glm::mat4& rotation) noexcept
{
	glm::vec3 eulerAngles;

	eulerAngles.x = atan2f(rotation[1][2], rotation[2][2]);
	eulerAngles.y = atan2f(-rotation[0][2], sqrtf(rotation[1][2] * rotation[1][2] + rotation[2][2] * rotation[2][2]));
	eulerAngles.z = atan2f(rotation[0][1], rotation[0][0]);
	eulerAngles.x = glm::degrees(eulerAngles.x);
	eulerAngles.y = glm::degrees(eulerAngles.y);
	eulerAngles.z = glm::degrees(eulerAngles.z);

	return eulerAngles;
}
