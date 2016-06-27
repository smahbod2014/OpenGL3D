#include "ShadowMapMasterRenderer.h"
#include "ShadowMapFramebuffer.h"
#include "Shader.h"
#include "ShadowBox.h"
#include "ShadowMapEntityRenderer.h"
#include "Camera.h"
#include "Entity.h"
#include "DirectionalLight.h"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Helpers.h"

ShadowMapMasterRenderer::ShadowMapMasterRenderer(Camera* camera)
{
	shader = new Shader();
	shader->load("Shaders/Shadow.vert", "Shaders/Shadow.frag");
	shader->bind();
	shader->setUniform1("modelTexture", 0);
	shader->unbind();
	shadowBox = new ShadowBox(&lightViewMatrix, camera);
	shadowFbo = new ShadowMapFramebuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	entityRenderer = new ShadowMapEntityRenderer(shader, &projectionViewMatrix);
	offset = createOffset();
}

ShadowMapMasterRenderer::~ShadowMapMasterRenderer()
{

}

void ShadowMapMasterRenderer::render(std::vector<Entity*> entities, DirectionalLight* dLight)
{
	shadowBox->update();
	prepare(dLight->getDirection(), shadowBox);
	entityRenderer->render(entities);
	finish();
}

glm::mat4 ShadowMapMasterRenderer::getToShadowMapSpaceMatrix()
{
	return offset * projectionViewMatrix;
}

void ShadowMapMasterRenderer::prepare(const glm::vec3& lightDirection, ShadowBox* box)
{
	updateOrthoProjectionMatrix(box->getWidth(), box->getHeight(), box->getLength());
	updateLightViewMatrix(lightDirection, box->getCenter());
	
	//std::cout << box->getWidth() << std::endl;
	projectionViewMatrix = projectionMatrix * lightViewMatrix;
	shadowFbo->bindFramebuffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	shader->bind();
}

void ShadowMapMasterRenderer::finish()
{
	shader->unbind();
	shadowFbo->unbindFramebuffer();
}

void ShadowMapMasterRenderer::updateLightViewMatrix(const glm::vec3& lightDirection, const glm::vec3& center)
{
	lightViewMatrix = glm::mat4();
	float pitch = acosf(glm::length(glm::vec2(lightDirection.x, lightDirection.z)));
	lightViewMatrix = glm::rotate(lightViewMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	float yaw = glm::degrees((atanf(lightDirection.x / lightDirection.z)));
	yaw = glm::radians(lightDirection.z > 0 ? yaw - 180 : yaw);
	lightViewMatrix = glm::rotate(lightViewMatrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	lightViewMatrix = glm::translate(lightViewMatrix, -center);
}

void ShadowMapMasterRenderer::updateOrthoProjectionMatrix(float width, float height, float length)
{
	projectionMatrix = glm::mat4();
	projectionMatrix[0][0] = 2.0f / width;
	projectionMatrix[1][1] = 2.0f / height;
	projectionMatrix[2][2] = -2.0f / length;
	projectionMatrix[3][3] = 1.0f;

	static bool printed = false;
	if (true || !printed) {
		//printMatrix(projectionMatrix);
		printed = true;
	}
}

glm::mat4 ShadowMapMasterRenderer::createOffset()
{
	glm::mat4 mat;
	mat[0][0] = 0.5f;
	mat[1][1] = 0.5f;
	mat[2][2] = 0.5f;
	mat[3][0] = 0.5f;
	mat[3][1] = 0.5f;
	mat[3][2] = 0.5f;
	return mat;
}

GLuint ShadowMapMasterRenderer::getShadowMapTexture()
{
	return shadowFbo->getShadowMapTexture();
}