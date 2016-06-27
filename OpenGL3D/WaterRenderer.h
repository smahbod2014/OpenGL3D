#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

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
	void setShadowMapTextureID(GLuint id) { shadowMapTexture = id; }
	void loadDirectionalLight(DirectionalLight* dLight);
	void loadShadowSpaceMatrix(const glm::mat4& matrix);
private:
	Shader* shader;
	Model* quad;
	WaterFramebuffers* waterfbos;
	GLuint shadowMapTexture = 0;
};

