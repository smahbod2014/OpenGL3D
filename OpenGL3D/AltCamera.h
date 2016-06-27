#pragma once

#include <glm/glm.hpp>

class AltCamera
{
public:
	AltCamera();
	~AltCamera();

	float getPitch() { return pitch; }
	float getYaw() { return yaw; }
	const glm::vec3& getPosition() { return position; }

private:
	glm::vec3 position;
	float pitch, yaw, roll;
};

