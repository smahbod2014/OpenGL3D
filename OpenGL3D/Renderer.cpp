#include "Renderer.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include "Window.h"
#include "DirectionalLight.h"
#include <glm/gtx/transform.hpp>

Renderer::Renderer()
{
	P = glm::perspective<float>(glm::radians<float>(60.0f), (float)Window::getWidth() / (float)Window::getHeight(), 0.1f, 1000.0f);
	setShader(Shader::createWaterDefault());
	m_Shader->bind();
	m_Shader->setUniform1("sampler", TEXTURE_INDEX);
	m_Shader->setUniformMatrix4("P", P);
	setCamera(nullptr);
}

Renderer::~Renderer()
{

}

void Renderer::setCamera(Camera* camera)
{
	if (m_Camera)
		delete m_Camera;

	if (camera == nullptr) {
		m_Camera = new Camera(glm::vec3(0, 0, 10),
							  glm::vec3(0, 0, 0),
							  glm::vec3(0, 1, 0));
		
	}
	else {
		m_Camera = camera;
	}

	updateCamera();
}

void Renderer::updateCamera()
{
	m_Shader->setUniformMatrix4("V", m_Camera->getInverseViewMatrix());
}

void Renderer::render(const Model& model)
{
	glBindVertexArray(model.m_Vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.m_Ibo);

	//glActiveTexture(TEXTURE_INDEX);
	//glBindTexture(GL_TEXTURE_2D, model.m_TexID);

	glDrawElements(GL_TRIANGLES, model.m_NumIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::loadDirectionalLight(DirectionalLight* dLight)
{
	m_Shader->bind();
	dLight->setUniforms(*m_Shader, "dLight");
}