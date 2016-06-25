#include "TerrainRenderer.h"
#include "Shader.h"
#include "Terrain.h"
#include "DirectionalLight.h"
#include "Camera.h"
#include "Helpers.h"
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>

TerrainRenderer::TerrainRenderer()
{
	shader = Shader::createTerrainDefault();
	shader->bind();
	shader->setUniformMatrix4("P", getDefaultProjectionMatrix());
	shader->setUniform1("backgroundTexture", 0);
	shader->setUniform1("rTexture", 1);
	shader->setUniform1("gTexture", 2);
	shader->setUniform1("bTexture", 3);
	shader->setUniform1("blendMap", 4);
	shader->unbind();
}


TerrainRenderer::~TerrainRenderer()
{
}

void TerrainRenderer::render(Terrain* terrain, Camera* camera)
{
	shader->bind();
	shader->setUniformMatrix4("V", camera->getInverseViewMatrix());
	glm::mat4 M;
	M[3][0] = terrain->x;
	M[3][1] = 0;
	M[3][2] = terrain->z;
	shader->setUniformMatrix4("M", M);

	glBindVertexArray(terrain->model->m_Vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->model->m_Ibo);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain->backTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain->rTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain->bTex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrain->gTex);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain->blendMap);

	glDrawElements(GL_TRIANGLES, terrain->model->m_NumIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader->unbind();
}

void TerrainRenderer::loadDirectionalLight(DirectionalLight* dLight)
{
	shader->bind();
	dLight->setUniforms(*shader, "dLight");
	shader->unbind();
}

void TerrainRenderer::loadClipPlane(float x, float y, float z, float w)
{
	shader->bind();
	shader->setUniform4("plane", glm::vec4(x, y, z, w));
	shader->unbind();
}