#include "SpotLight.h"
#include "Shader.h"
#include "Window.h"
#include <glm/gtx/transform.hpp>

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& color, const glm::vec3& direction, float cutoff, float ca, float la, float qa) :
	PointLight(position, color, ca, la, qa)
{
	setDirection(direction);
	setCutoff(cutoff);
	m_ShadowCamera.set(position, position + m_Direction, glm::vec3(0, 1, 0));
	//m_Projection = glm::perspective<float>(glm::radians<float>(m_Cutoff * 2.0f), (float)Window::getWidth() / (float)Window::getHeight(), 0.1f, 1000.0f);
	m_Projection = glm::perspective<float>(glm::radians<float>(m_Cutoff * 2.0f), 1.0f, 0.1f, 1000.0f);
}

SpotLight::SpotLight(const glm::vec3& position, unsigned int color, const glm::vec3& direction, float cutoff, float ca, float la, float qa) :
	PointLight(position, color, ca, la, qa)
{
	setDirection(direction);
	setCutoff(cutoff);
	m_ShadowCamera.set(position, position + m_Direction, glm::vec3(0, 1, 0));
	//m_Projection = glm::perspective<float>(glm::radians<float>(m_Cutoff * 2.0f), (float)Window::getWidth() / (float)Window::getHeight(), 0.1f, 1000.0f);
	m_Projection = glm::perspective<float>(glm::radians<float>(m_Cutoff * 2.0f), 1.0f, 0.1f, 1000.0f);
}

SpotLight::~SpotLight()
{
}

void SpotLight::setDirection(const glm::vec3& direction)
{
	m_Direction = glm::normalize(direction);
}

void SpotLight::setCutoff(float degrees)
{
	m_Cutoff = degrees;
	m_AdjustedCutoff = cosf(glm::radians<float>(degrees));
}

void SpotLight::setUniforms(Shader& shader, const std::string& prefix)
{
	PointLight::setUniforms(shader, prefix + ".base");
	shader.setUniform3(prefix + ".direction", m_Direction);
	shader.setUniform1(prefix + ".cutoff", m_AdjustedCutoff);
}

void SpotLight::translate(const glm::vec3& amount)
{
	PointLight::translate(amount);
	m_ShadowCamera.translate(amount);
}

void SpotLight::setPosition(const glm::vec3& position)
{
	PointLight::translate(position);
	m_ShadowCamera.setPosition(position);
}

void SpotLight::rotate(const glm::vec3& axis, float degrees)
{
	m_Direction = glm::normalize(glm::vec3(glm::rotate(glm::radians<float>(degrees), axis) * glm::vec4(m_Direction, 0.0)));
	m_ShadowCamera.rotate(axis, degrees);
}