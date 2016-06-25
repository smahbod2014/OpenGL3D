#pragma once

#include <glm/glm.hpp>

class Camera2
{
public:
	Camera2();
	~Camera2();

public:
	glm::vec3 position;
	float pitch, yaw, roll;
};

