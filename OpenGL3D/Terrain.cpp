#include "Terrain.h"
#include "TextureManager.h"
#include "Model.h"
#include "Helpers.h"
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
	heights = new float*[heightMap->height];
	for (int i = 0; i < heightMap->height; i++)
		heights[i] = new float[heightMap->height];
	generateTerrain();
}

Terrain::Terrain(int gridX, int gridZ, const std::string& backAlias, const std::string& rTexAlias,
				 const std::string& gTexAlias, const std::string& bTexAlias, const std::string& blendMapAlias)
{
	backTex = TextureManager::getTexture(backAlias);
	rTex = TextureManager::getTexture(rTexAlias);
	bTex = TextureManager::getTexture(gTexAlias);
	gTex = TextureManager::getTexture(bTexAlias);
	blendMap = TextureManager::getTexture(blendMapAlias);
	heightGenerator = new HeightGenerator(30.0f, VERTEX_COUNT);
	x = gridX * TERRAIN_SIZE;
	z = gridZ * TERRAIN_SIZE;
	model = new Model();
	heights = new float*[VERTEX_COUNT];
	for (int i = 0; i < VERTEX_COUNT; i++)
		heights[i] = new float[VERTEX_COUNT];
	generateTerrainProcedural();
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
			heights[j][i] = vertices[vertexPointer * 3 + 1] = getHeight(j, i);
			//if (heights[j][i] > -39) std::cout << heights[j][i] << std::endl;
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

void Terrain::generateTerrainProcedural()
{
	//learn this someday!
	int TERRAIN_VERTEX_COUNT = VERTEX_COUNT;

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
			heights[j][i] = vertices[vertexPointer * 3 + 1] = getHeightProcedural(j, i);
			vertices[vertexPointer * 3 + 2] = (float)i / ((float)TERRAIN_VERTEX_COUNT - 1) * TERRAIN_SIZE;
			glm::vec3 normal = calculateNormalProcedural(j, i);
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
	
	if (x < 0 || x >= heightMap->width || z < 0 || z >= heightMap->height) {
		//std::cout << "x: " << x << ", z: " << z << std::endl;
		return 0.0f;
	}

	float height = (float)heightMap->getRed(x, z);
	height /= 128.0f;
	height -= 1.0f;
	height *= TERRAIN_MAX_HEIGHT;
	return height;
}

float Terrain::getHeightProcedural(int x, int z)
{
	return heightGenerator->generateHeight(x, z);
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

glm::vec3 Terrain::calculateNormalProcedural(int x, int z)
{
	float heightL = getHeightProcedural(x - 1, z);
	float heightR = getHeightProcedural(x + 1, z);
	float heightD = getHeightProcedural(x, z - 1);
	float heightU = getHeightProcedural(x, z + 1);
	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	normal = glm::normalize(normal);
	return normal;
}

float Terrain::getHeightAtLocation(float worldX, float worldZ)
{
	float terrainX = worldX - this->x;
	float terrainZ = worldZ - this->z;
	float gridSquareSize;
	if (heightMap)
		gridSquareSize = TERRAIN_SIZE / ((float)(heightMap->height - 1));
	else
		gridSquareSize = TERRAIN_SIZE / ((float)(VERTEX_COUNT - 1));

	int gridX = (int)floorf(terrainX / gridSquareSize);
	int gridZ = (int)floorf(terrainZ / gridSquareSize);

	if (heightMap) {
		if (gridX >= heightMap->height - 1 || gridZ >= heightMap->height - 1 || gridX < 0 || gridZ < 0) {
			//std::cout << "Nope: " << answer << std::endl;
			return 0;
		}
	}
	else
		if (gridX >= VERTEX_COUNT - 1 || gridZ >= VERTEX_COUNT - 1 || gridX < 0 || gridZ < 0)
			return 0;

	float xCoord = fmodf(terrainX, gridSquareSize) / gridSquareSize;
	float zCoord = fmodf(terrainZ, gridSquareSize) / gridSquareSize;
	float answer;
	if (xCoord <= (1 - zCoord)) {
		answer = barryCentric(glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1,
			heights[gridX + 1][gridZ], 0), glm::vec3(0,
			heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}
	else {
		float s = heights[gridX][gridZ + 1];
		answer = barryCentric(glm::vec3(1.0f, heights[gridX + 1][gridZ], 0), glm::vec3(1.0f,
			heights[gridX + 1][gridZ + 1], 1.0f), glm::vec3(0.0f,
			heights[gridX][gridZ + 1], 1.0f), glm::vec2(xCoord, zCoord));
	}
	std::cout << "Answer: " << answer << std::endl;
	return answer;
}