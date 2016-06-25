#pragma once

#include <GL/glew.h>
#include <string>

class WaterTile
{
public:
	WaterTile(float centerX, float centerZ, float height, float size, float waveSpeed, const std::string& dudvAlias);
	~WaterTile();

	void update(float dt);
public:
	float centerX, centerZ, height, size, waveSpeed;
	float moveFactor = 0.0f;
	GLuint dudvID;
};

