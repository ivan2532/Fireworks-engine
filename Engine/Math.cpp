#include "Math.hpp"

void Math::DecomposeTransform(glm::mat4 trans, glm::vec3* translation, glm::vec3* scale, glm::mat4* rotation)
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

glm::vec3 Math::EulerAnglesFromRotation(const glm::mat4& rotation)
{
	float sy = sqrt(rotation[0][0] * rotation[0][0] + rotation[1][0] * rotation[1][0]);
	glm::vec3 eulerAngles;

	if (sy >= 1e-6)
	{
		eulerAngles.x = -atan2(rotation[2][1], rotation[2][2]);
		eulerAngles.y = atan2(-rotation[2][0], sy);
		eulerAngles.z = atan2(rotation[1][0], rotation[0][0]);
	}
	else
	{
		eulerAngles.x = -atan2(-rotation[1][2], rotation[1][1]);
		eulerAngles.y = atan2(-rotation[2][0], sy);
		eulerAngles.z = 0;
	}

	eulerAngles.x = glm::degrees(eulerAngles.x);
	eulerAngles.y = glm::degrees(eulerAngles.y);
	eulerAngles.z = glm::degrees(eulerAngles.z);

	return eulerAngles;
}
