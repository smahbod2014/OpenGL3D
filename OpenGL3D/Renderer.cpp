#include "Renderer.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include "Window.h"
#include "Helpers.h"
#include "DirectionalLight.h"
#include <glm/gtx/transform.hpp>

Renderer::Renderer()
{
	setShader(Shader::createRegularDefault());
	m_Shader->bind();
	m_Shader->setUniform1("sampler", TEXTURE_INDEX);
	m_Shader->setUniformMatrix4("P", getDefaultProjectionMatrix());
	m_Shader->unbind();
}

Renderer::~Renderer()
{

}

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