#include "Renderer.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include "Window.h"
#include "Helpers.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include <glm/gtx/transform.hpp>

Renderer::Renderer()
{
	setShader(Shader::createRegularDefault());
	m_Shader->bind();
	m_Shader->setUniform1("diffuseMap", 0);
	m_Shader->setUniform1("normalMap", 1);
	m_Shader->setUniform1("specularMap", 2);
	//m_Shader->setUniform1("shadowMap", 1);
	m_Shader->setUniformMatrix4("P", getDefaultProjectionMatrix());
	m_Shader->unbind();
}

Renderer::~Renderer()
{

}

//DEPRECATED
void Renderer::render(const Model& model, Camera* camera)
{
	m_Shader->bind();
	m_Shader->setUniformMatrix4("V", camera->getInverseViewMatrix());

	glBindVertexArray(model.m_Vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.m_Ibo);

	//glActiveTexture(TEXTURE_INDEX);
	//glBindTexture(GL_TEXTURE_2D, model.m_TexID);

	glDisable(GL_CULL_FACE);

	glDrawElements(GL_TRIANGLES, model.m_NumIndices, GL_UNSIGNED_INT, 0);

	glEnable(GL_CULL_FACE);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_Shader->unbind();
}

void Renderer::render(std::vector<Entity*> entities, Camera* camera)
{
	m_Shader->bind();
	m_Shader->setUniformMatrix4("V", camera->getInverseViewMatrix());
	m_Shader->setUniform3("cameraPosition", camera->getPosition());

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	for (Entity* entity : entities) {
		m_Shader->setUniformMatrix4("M", entity->getTransformation());
		m_Shader->setUniform1("specularDamper", entity->getSpecularDamper());
		m_Shader->setUniform1("specularReflectivity", entity->getSpecularReflectivity());
		glBindVertexArray(entity->getModel()->m_Vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entity->getModel()->m_Ibo);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity->getTexID());

		if (entity->getNormalMapID()) {
			m_Shader->setUniform1("hasNormalMap", 1.0f);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, entity->getNormalMapID());
		}
		else {
			m_Shader->setUniform1("hasNormalMap", 0.0f);
		}

		if (entity->getSpecularMapID()) {
			m_Shader->setUniform1("hasSpecularMap", 1.0f);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, entity->getSpecularMapID());
		}
		else {
			m_Shader->setUniform1("hasSpecularMap", 0.0f);
		}

		glDisable(GL_CULL_FACE);

		glDrawElements(GL_TRIANGLES, entity->getModel()->m_NumIndices, GL_UNSIGNED_INT, 0);

		glEnable(GL_CULL_FACE);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	m_Shader->unbind();
}

void Renderer::loadDirectionalLight(DirectionalLight* dLight)
{
	m_Shader->bind();
	dLight->setUniforms(*m_Shader, "dLight");
	m_Shader->unbind();
}

void Renderer::loadClipPlane(float x, float y, float z, float w)
{
	m_Shader->bind();
	m_Shader->setUniform4("plane", glm::vec4(x, y, z, w));
	m_Shader->unbind();
}

void Renderer::loadShadowSpaceMatrix(const glm::mat4& matrix)
{
	m_Shader->bind();
	m_Shader->setUniformMatrix4("toShadowMapSpace", matrix);
	m_Shader->unbind();
}