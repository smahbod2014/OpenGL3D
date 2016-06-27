#include "WaterRenderer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Helpers.h"
#include "WaterTile.h"
#include "WaterFramebuffers.h"
#include "DirectionalLight.h"
#include "ShadowBox.h"
#include "ShadowMapMasterRenderer.h"

WaterRenderer::WaterRenderer(WaterFramebuffers* waterfbos)
{
	this->waterfbos = waterfbos;

	shader = Shader::createWaterDefault();
	shader->bind();
	shader->setUniformMatrix4("P", getDefaultProjectionMatrix());
	shader->setUniform1("reflectionTexture", 0);
	shader->setUniform1("refractionTexture", 1);
	shader->setUniform1("dudvMap", 2);
	shader->setUniform1("normalMap", 3);
	shader->setUniform1("depthMap", 4);
	shader->unbind();

	quad = new Model();
	float vertices[] = { 
		-1, 0, -1,
		-1, 0, 1, 
		1, 0, -1, 
		1, 0, -1, 
		-1, 0, 1, 
		1, 0, 1 };
	quad->generateVerticesOnly(vertices, 6);
}


WaterRenderer::~WaterRenderer()
{
}

void WaterRenderer::render(WaterTile* water, Camera* camera)
{
	shader->bind();
	shader->setUniformMatrix4("V", camera->getInverseViewMatrix());
	glm::mat4 M = createTransformation(water->centerX, water->height, water->centerZ, 0, 1, 0, 0, water->size, 1, water->size);
	shader->setUniformMatrix4("M", M);
	shader->setUniform1("moveFactor", water->moveFactor);
	shader->setUniform3("cameraPosition", camera->getPosition());

	glBindVertexArray(quad->m_Vao);
	glEnableVertexAttribArray(0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterfbos->reflectionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, waterfbos->refractionTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, water->dudvID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, water->normalMapID);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, waterfbos->refractionDepthTexture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	shader->unbind();
}

void WaterRenderer::loadDirectionalLight(DirectionalLight* dLight)
{
	shader->bind();
	dLight->setUniforms(*shader, "dLight");
	shader->unbind();
}

void WaterRenderer::loadShadowSpaceMatrix(const glm::mat4& matrix)
{
	shader->bind();
	shader->setUniformMatrix4("toShadowMapSpace", matrix);
	shader->unbind();
}