#include "Terrain.h"
#include "TextureManager.h"
#include "Model.h"
#include <iostream>

Terrain::Terrain(int gridX, int gridZ, const std::string& backAlias, const std::string& rTexAlias,
				 const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias,
				 const std::string& heightMapPath)
{
	backTex = TextureManager::getTexture(backAlias);
	rTex = TextureManager::getTexture(rTexAlias);
	bTex = TextureManager::getTexture(gTexAlias);
	gTex = TextureManager::getTexture(bTexAlias);
	blendMap = TextureManager::getTexture(blendMapAlias);
	heightMap = new TextureData(heightMapPath);
	x = gridX * TERRAIN_SIZE;
	z = gridZ * TERRAIN_SIZE;
	model = new Model();
	generateTerrain();
}


Terrain::~Terrain()
{
}

void Terrain::generateTerrain()
{
	//learn this someday!
	int TERRAIN_VERTEX_COUNT = heightMap->height;

	int count = TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT;
	int indexCount = 6 * (TERRAIN_VERTEX_COUNT - 1)*(TERRAIN_VERTEX_COUNT - 1);
	float* vertices = new float[count * 3];
	float* normals = new float[count * 3];
	float* textureCoords = new float[count * 2];
	int* indices = new int[indexCount];
	int vertexPointer = 0;
	for (int i = 0; i<TERRAIN_VERTEX_COUNT; i++){
		for (int j = 0; j<TERRAIN_VERTEX_COUNT; j++){
			vertices[vertexPointer * 3] = (float)j / ((float)TERRAIN_VERTEX_COUNT - 1) * TERRAIN_SIZE;
			vertices[vertexPointer * 3 + 1] = getHeight(j, i);
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)TERRAIN_VERTEX_COUNT - 1) * TERRAIN_SIZE;
			glm::vec3 normal = calculateNormal(j, i);
			normals[vertexPointer * 3] = normal.x;
			normals[vertexPointer * 3 + 1] = normal.y;
			normals[vertexPointer * 3 + 2] = normal.z;
			textureCoords[vertexPointer * 2] = (float)j / ((float)TERRAIN_VERTEX_COUNT - 1);
			textureCoords[vertexPointer * 2 + 1] = (float)i / ((float)TERRAIN_VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz<TERRAIN_VERTEX_COUNT - 1; gz++){
		for (int gx = 0; gx<TERRAIN_VERTEX_COUNT - 1; gx++){
			int topLeft = (gz*TERRAIN_VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*TERRAIN_VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
	
	model->loadArbitrary(vertices, textureCoords, normals, (unsigned int*)indices, count, indexCount);
}

float Terrain::getHeight(int x, int z)
{
	/*float height = heightMap->getPixel(x, z);
	height += MAX_PIXEL_COLOR / 2.0f;
	height /= MAX_PIXEL_COLOR / 2.0f;
	height *= TERRAIN_MAX_HEIGHT;
	return height; ???*/
	
	if (x < 0 || x >= heightMap->width || z < 0 || z >= heightMap->height)
		return 0.0f;

	float height = (float)heightMap->getRed(x, z);
	height /= 128.0f;
	height -= 1.0f;
	height *= TERRAIN_MAX_HEIGHT;
	return height;
}

glm::vec3 Terrain::calculateNormal(int x, int z)
{
	float heightL = getHeight(x - 1, z);
	float heightR = getHeight(x + 1, z);
	float heightD = getHeight(x, z - 1);
	float heightU = getHeight(x, z + 1);
	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	normal = glm::normalize(normal);
	return normal;
}