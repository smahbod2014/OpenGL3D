#include "WaterTile.h"
#include "TextureManager.h"

WaterTile::WaterTile(float centerX, float centerZ, float height, float size, float waveSpeed, const std::string& dudvAlias, const std::string& normalMapAlias)
{
	this->centerX = centerX;
	this->centerZ = centerZ;
	this->height = height;
	this->size = size;
	this->waveSpeed = waveSpeed;
	dudvID = TextureManager::getTexture(dudvAlias);
	normalMapID = TextureManager::getTexture(normalMapAlias);
}

WaterTile::~WaterTile()
{
}

void WaterTile::update(float dt)
{
	moveFactor += waveSpeed * dt;
	moveFactor = fmodf(moveFactor, 1.0f);
}
