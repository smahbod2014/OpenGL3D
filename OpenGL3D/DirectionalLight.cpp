#include "DirectionalLight.h"
#include "Shader.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float size)
	: BaseLight(color)
{
	m_Direction = glm::normalize(direction);
	//m_Camera.set(glm::vec3(0, 0, 0), m_Direction, glm::vec3(0, 1, 0));
	m_HalfSize = size / 2.0f;
	m_Projection = glm::ortho<float>(-m_HalfSize, m_HalfSize, -m_HalfSize, m_HalfSize, -m_HalfSize, m_HalfSize);
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, unsigned int color, float size)
	: BaseLight(color)
{
	m_Direction = glm::normalize(direction);
	m_HalfSize = size / 2.0f;
	glm::vec3 pos(0, 0, 0);
	//m_Camera.set(pos, pos + m_Direction, glm::vec3(0, 1, 0));
	m_Projection = glm::ortho<float>(-m_HalfSize, m_HalfSize, -m_HalfSize, m_HalfSize, -m_HalfSize, m_HalfSize);
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::setUniforms(Shader& shader, const std::string& prefix)
{
	BaseLight::setUniforms(shader, prefix + ".base");
	shader.setUniform3(prefix + ".direction", m_Direction);
}

void DirectionalLight::setCameraPosition(const Camera& mainCamera)
{
	//m_Camera.setPosition(mainCamera.getPosition() + mainCamera.getForward() * m_HalfSize / 2.0f);
}