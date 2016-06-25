#pragma once

class WaterTile
{
public:
	WaterTile(float centerX, float centerZ, float height, float size);
	~WaterTile();

public:
	float centerX, centerZ, height, size;
};

