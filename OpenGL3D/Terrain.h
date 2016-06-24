#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "TextureManager.h"

#define TERRAIN_SIZE 500
#define TERRAIN_MAX_HEIGHT 40.0f
#define MAX_PIXEL_COLOR 256 * 256 * 256

class Model;

class Terrain
{
	friend class TerrainRenderer;
public:
	Terrain(int gridX, int gridZ, const std::string& backAlias, const std::string& rTexAlias,
			const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias,
			const std::string& heightMapPath);
	~Terrain();

	void generateTerrain();

private:
	float getHeight(int x, int z);
	glm::vec3 calculateNormal(int x, int z);
private:
	float x, z;
	Model* model;
	GLuint backTex, rTex, gTex, bTex, blendMap;
	TextureData* heightMap;
};

