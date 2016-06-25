#pragma once

class Shader;
class Model;
class Camera;
class WaterTile;
class WaterFramebuffers;
class DirectionalLight;

class WaterRenderer
{
public:
	WaterRenderer(WaterFramebuffers* waterfbos);
	~WaterRenderer();

	void render(WaterTile* water, Camera* camera);
	void loadDirectionalLight(DirectionalLight* dLight);

private:
	Shader* shader;
	Model* quad;
	WaterFramebuffers* waterfbos;
};

