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
	for (Entity* entity : entities) {
		glm::mat4 mvp = *projectionViewMatrix * entity->getTransformation();
		shader->setUniformMatrix4("MVP", mvp);

		//if entity's texture has transparency, disable back face culling
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity->getTexID());
		glBindVertexArray(entity->getModel()->m_Vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity->getModel()->m_Ibo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, entity->getModel()->m_NumIndices, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);

		//if entity's texture had transparency, re-enable back face culling
	}
}
