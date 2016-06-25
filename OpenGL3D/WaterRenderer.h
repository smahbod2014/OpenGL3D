#pragma once

class Shader;
class Model;
class Camera;
class WaterTile;
class WaterFramebuffers;

class WaterRenderer
{
public:
	WaterRenderer(WaterFramebuffers* waterfbos);
	~WaterRenderer();

	void render(WaterTile* water, Camera* camera);

private:
	Shader* shader;
	Model* quad;
	WaterFramebuffers* waterfbos;
};

