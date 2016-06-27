#pragma once

#include <glm/glm.hpp>
#include <vector>

#define OFFSET 10.0f
#define SHADOW_DISTANCE 100.0f
#define UP glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
#define FORWARD glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)

class Camera;

class ShadowBox
{
public:
	ShadowBox(glm::mat4* lightViewMatrix, Camera* camera);
	~ShadowBox();

	void update();
	void calculateWidthsAndHeights();
	glm::mat4 calculateCameraRotationMatrix();
	glm::vec4 calculateLightSpaceFrustumCorner(const glm::vec3& startPoint, const glm::vec3& direction, float width);
	void calculateFrustumVertices(const glm::mat4& rotation, const glm::vec3& forwardVector,
													const glm::vec3& centerNear, const glm::vec3& centerFar, glm::vec4* points);

	glm::vec3 getCenter();
	float getWidth() { return maxX - minX; }
	float getHeight() { return maxY - minY; }
	float getLength() { return maxZ - minZ; }
	float getAspectRatio();

private:
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	float farHeight, farWidth, nearHeight, nearWidth;
	glm::mat4* lightViewMatrix;
	Camera* camera;
};

