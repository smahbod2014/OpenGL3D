#include "GLMHelper.h"

glm::mat4 translation(float x, float y, float z)
{
	glm::mat4 result(1.0);
	result[3][0] = x;
	result[3][1] = y;
	result[3][2] = z;
	return result;
}

glm::mat4 translation(const glm::vec3& trans)
{
	glm::mat4 result(1.0);
	result[3][0] = trans.x;
	result[3][1] = trans.y;
	result[3][2] = trans.z;
	return result;
}