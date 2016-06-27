#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#define SHADOW_MAP_SIZE 8192
#define PCF_COUNT 2

class ShadowMapFramebuffer;
class Shader;
class ShadowBox;
class ShadowMapEntityRenderer;
class Camera;
class Entity;
class DirectionalLight;

class ShadowMapMasterRenderer
{
public:
	ShadowMapMasterRenderer(Camera* camera);
	~ShadowMapMasterRenderer();

	void render(std::vector<Entity*> entities, DirectionalLight* dLight);
	glm::mat4 getToShadowMapSpaceMatrix();
	glm::mat4 getLightSpaceTransform() { return lightViewMatrix; }
	void prepare(const glm::vec3& lightDirection, ShadowBox* box);
	void finish();
	void updateLightViewMatrix(const glm::vec3& lightDirection, const glm::vec3& center);
	void updateOrthoProjectionMatrix(float width, float height, float length);
	glm::mat4 createOffset();
	GLuint getShadowMapTexture();

private:
	ShadowMapFramebuffer* shadowFbo;
	Shader* shader;
	ShadowBox* shadowBox;
	ShadowMapEntityRenderer* entityRenderer;
	glm::mat4 projectionMatrix, lightViewMatrix, projectionViewMatrix, offset;
};

