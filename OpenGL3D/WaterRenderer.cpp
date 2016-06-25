#include "WaterRenderer.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Helpers.h"
#include "WaterTile.h"


WaterRenderer::WaterRenderer()
{
	shader = Shader::createWaterDefault();
	shader->bind();
	shader->setUniformMatrix4("P", getDefaultProjectionMatrix());
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

	glBindVertexArray(quad->m_Vao);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	shader->unbind();
}
