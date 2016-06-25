#pragma once

class Shader;
class Model;
class Camera;
class WaterTile;

class WaterRenderer
{
public:
	WaterRenderer();
	~WaterRenderer();

	void render(WaterTile* water, Camera* camera);

private:
	Shader* shader;
	Model* quad;
};

