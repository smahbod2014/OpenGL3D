#include "WaterTile.h"
#include "TextureManager.h"

WaterTile::WaterTile(float centerX, float centerZ, float height, float size, float waveSpeed, const std::string& dudvAlias)
{
	this->centerX = centerX;
	this->centerZ = centerZ;
	this->height = height;
	this->size = size;
	this->waveSpeed = waveSpeed;
	dudvID = TextureManager::getTexture(dudvAlias);
}


WaterTile::~WaterTile()
{
}

void WaterTile::update(float dt)
{
	moveFactor += waveSpeed * dt;
	moveFactor = fmodf(moveFactor, 1.0f);
}
