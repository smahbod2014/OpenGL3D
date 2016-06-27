#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "TextureManager.h"
#include "HeightGenerator.h"

#define TERRAIN_SIZE 64
#define TERRAIN_MAX_HEIGHT 40.0f
#define VERTEX_COUNT 96

class Model;

class Terrain
{
	friend class TerrainRenderer;
public:
	Terrain(float gridX, float gridZ, float size, float maxHeight, const std::string& backAlias, const std::string& rTexAlias,
			const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias,
			const std::string& heightMapPath);
	Terrain(float gridX, float gridZ, float size, float maxHeight, int vertexCount, const std::string& backAlias, const std::string& rTexAlias,
			const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias);
	~Terrain();

	void generateTerrain();
	void generateTerrainProcedural();

	float getHeightAtLocation(float worldX, float worldZ);
	float getX() { return x; }
	float getZ() { return z; }
	float getSize() { return size; }

private:
	float getHeight(int x, int z);
	float getHeightProcedural(int x, int z);
	glm::vec3 calculateNormal(int x, int z);
	glm::vec3 calculateNormalProcedural(int x, int z);
private:
	float x, z, size;
	float** heights;
	Model* model;
	GLuint backTex, rTex, gTex, bTex, blendMap;
	TextureData* heightMap;
	HeightGenerator* heightGenerator;
	int vertexCount;
	float maxHeight;
};

