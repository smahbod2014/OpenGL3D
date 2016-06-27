#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

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
	void setShadowMapTextureID(GLuint id) { shadowMapTexture = id; }
	void loadDirectionalLight(DirectionalLight* dLight);
	void loadClipPlane(float x, float y, float z, float w);
	void loadShadowSpaceMatrix(const glm::mat4& matrix);
private:
	Shader* shader;
	GLuint shadowMapTexture;
};

