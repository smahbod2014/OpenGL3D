#include "Geode.h"
#include "Model.h"
#include "ModelCache.h"
#include "Renderer.h"
#include "Shader.h"
#include "Constants.h"

Geode::Geode(const std::string& alias, Renderer* renderer)
{
	m_Model = ModelCache::getModel(alias);
	m_Renderer = renderer;
}

Geode::~Geode()
{

}

void Geode::draw(const glm::mat4& matrix)
{
	if (m_RenderPassIndex >= 0)
	{
		//do I need this??? doesn't look like it
		//m_Renderer->m_Shader->bind();
		//m_Renderer->m_Shader->setUniformMatrix4("lightM[" + std::to_string(m_RenderPassIndex) + "]", matrix);
		m_Renderer->m_ShadowShader->bind();
		m_Renderer->m_ShadowShader->setUniformMatrix4("lightM", matrix);
	}
	else
	{
		m_Renderer->m_Shader->bind();
		m_Renderer->m_Shader->setUniformMatrix4("M", matrix);

		//the binding of the texture here and NOT in the renderer is crucial!
		glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX);
		glBindTexture(GL_TEXTURE_2D, m_TexID);
	}

	m_Renderer->render(*m_Model);
}