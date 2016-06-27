#include "ShadowBox.h"
#include "Camera.h"
#include "Window.h"
#include "Helpers.h"

ShadowBox::ShadowBox(glm::mat4* lightViewMatrix, Camera* camera)
{
	this->lightViewMatrix = lightViewMatrix;
	this->camera = camera;
	calculateWidthsAndHeights();
}

ShadowBox::~ShadowBox()
{
	
}

void ShadowBox::update()
{
	glm::mat4 rotation = calculateCameraRotationMatrix();
	glm::vec3 forwardVector(rotation * FORWARD);

	glm::vec3 toFar = forwardVector * SHADOW_DISTANCE;
	glm::vec3 toNear = forwardVector * NEAR_PLANE;
	glm::vec3 centerNear = camera->getPosition() + toNear;
	glm::vec3 centerFar = camera->getPosition() + toFar;

	glm::vec4 points[8];
	calculateFrustumVertices(rotation, forwardVector, centerNear,
							 centerFar, points);

	boolean first = true;
	for (glm::vec4& point : points) {
		if (first) {
			minX = point.x;
			maxX = point.x;
			minY = point.y;
			maxY = point.y;
			minZ = point.z;
			maxZ = point.z;
			first = false;
			continue;
		}
		if (point.x > maxX) {
			maxX = point.x;
		}
		else if (point.x < minX) {
			minX = point.x;
		}
		if (point.y > maxY) {
			maxY = point.y;
		}
		else if (point.y < minY) {
			minY = point.y;
		}
		if (point.z > maxZ) {
			maxZ = point.z;
		}
		else if (point.z < minZ) {
			minZ = point.z;
		}
	}

	maxZ += OFFSET;
}

void ShadowBox::calculateWidthsAndHeights()
{
	farWidth = (float)(SHADOW_DISTANCE * tanf(deg2rad(FOV)));
	nearWidth = (float)(NEAR_PLANE * tanf(deg2rad(FOV)));
	std::cout << "Nearwidth is " << nearWidth << std::endl;
	farHeight = farWidth / getAspectRatio();
	nearHeight = nearWidth / getAspectRatio();
}

glm::mat4 ShadowBox::calculateCameraRotationMatrix()
{
	/*glm::mat4 view;
	view = glm::rotate(view, deg2rad(-camera->getPitch()), glm::vec3(1, 0, 0));
	view = glm::rotate(view, deg2rad(-camera->getYaw()), glm::vec3(0, 1, 0));
	return view;*/
	glm::mat4 view = glm::inverse(camera->getInverseViewMatrix());
	view[3][0] = 0.0f;
	view[3][1] = 0.0f;
	view[3][2] = 0.0f;
	return view;
}

glm::vec4 ShadowBox::calculateLightSpaceFrustumCorner(const glm::vec3& startPoint, const glm::vec3& direction, float width)
{
	glm::vec3 point = startPoint + direction * width;
	glm::vec4 point2(point, 1.0f);
	return *lightViewMatrix * point2;
}

void ShadowBox::calculateFrustumVertices(const glm::mat4& rotation, const glm::vec3& forwardVector,
										 const glm::vec3& centerNear, const glm::vec3& centerFar, glm::vec4* points)
{
	glm::vec3 upVector(rotation * UP);
	glm::vec3 rightVector = glm::cross(forwardVector, upVector);
	glm::vec3 downVector = -upVector;
	glm::vec3 leftVector = -rightVector;
	glm::vec3 farTop = centerFar + upVector * farHeight;
	glm::vec3 farBottom = centerFar + downVector * farHeight;
	glm::vec3 nearTop = centerNear + upVector * nearHeight;
	glm::vec3 nearBottom = centerNear + downVector * nearHeight;
	points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
	points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
	points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
	points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
	points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
	points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
	points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
	points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
}

glm::vec3 ShadowBox::getCenter()
{
	float x = (minX + maxX) / 2.0f;
	float y = (minY + maxY) / 2.0f;
	float z = (minZ + maxZ) / 2.0f;
	glm::vec4 cen(x, y, z, 1.0f);
	glm::mat4 invertedLight = glm::inverse(*lightViewMatrix);
	return glm::vec3(invertedLight * cen);
}

float ShadowBox::getAspectRatio()
{
	return (float)Window::getWidth() / (float)Window::getHeight();
}