#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "TextureManager.h"
#include "HeightGenerator.h"

#define TERRAIN_SIZE 64
#define TERRAIN_MAX_HEIGHT 40.0f
#define VERTEX_COUNT 16
#define MAX_PIXEL_COLOR 256 * 256 * 256

class Model;

class Terrain
{
	friend class TerrainRenderer;
public:
	Terrain(int gridX, int gridZ, const std::string& backAlias, const std::string& rTexAlias,
			const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias,
			const std::string& heightMapPath);
	Terrain(int gridX, int gridZ, const std::string& backAlias, const std::string& rTexAlias,
			const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias);
	~Terrain();

	void generateTerrain();
	void generateTerrainProcedural();

	float getHeightAtLocation(float worldX, float worldZ);
	float getX() { return x; }
	float getZ() { return z; }

private:
	float getHeight(int x, int z);
	float getHeightProcedural(int x, int z);
	glm::vec3 calculateNormal(int x, int z);
	glm::vec3 calculateNormalProcedural(int x, int z);
private:
	float x, z;
	float** heights;
	Model* model;
	GLuint backTex, rTex, gTex, bTex, blendMap;
	TextureData* heightMap;
	HeightGenerator* heightGenerator;
};

