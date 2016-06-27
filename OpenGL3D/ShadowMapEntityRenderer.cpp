#include "ShadowMapEntityRenderer.h"
#include "Shader.h"
#include "Entity.h"
#include "Model.h"
#include "Helpers.h"

ShadowMapEntityRenderer::ShadowMapEntityRenderer(Shader* shader, glm::mat4* projectionViewMatrix)
{
	this->shader = shader;
	this->projectionViewMatrix = projectionViewMatrix;
}


ShadowMapEntityRenderer::~ShadowMapEntityRenderer()
{
}

void ShadowMapEntityRenderer::render(const std::vector<Entity*> entities)
{
	shader->bind();
	for (Entity* entity : entities) {
		glm::mat4 mvp = *projectionViewMatrix * entity->getTransformation();
		shader->setUniformMatrix4("MVP", mvp);
		
		glBindVertexArray(entity->getModel()->m_Vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity->getModel()->m_Ibo);
		glEnableVertexAttribArray(0);
		glDrawElements(GL_TRIANGLES, entity->getModel()->m_NumIndices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	shader->unbind();
}
