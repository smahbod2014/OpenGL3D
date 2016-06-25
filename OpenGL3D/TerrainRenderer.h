#pragma once

class Shader;
class Terrain;
class DirectionalLight;
class Camera;

class TerrainRenderer
{
public:
	TerrainRenderer();
	~TerrainRenderer();

	void render(Terrain* terrain, Camera* camera);
	void loadDirectionalLight(DirectionalLight* dLight);
	void loadClipPlane(float x, float y, float z, float w);
private:
	Shader* shader;
};

