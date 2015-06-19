#include "DirectionalLight.h"
#include "Shader.h"
#include <glm/gtx/transform.hpp>

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color)
	: BaseLight(color)
{
	m_Direction = glm::normalize(direction);
	m_Camera.set(glm::vec3(0, 0, 0), m_Direction, glm::vec3(0, 1, 0));
	m_Projection = glm::ortho<float>(-15, 15, 0, 30, -15.0f, 15);
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, unsigned int color)
	: BaseLight(color)
{
	m_Direction = glm::normalize(direction);
	m_Camera.set(glm::vec3(0, 0, 0), m_Direction, glm::vec3(0, 1, 0));
	m_Projection = glm::ortho<float>(-15, 15, 0, 30, -15.0f, 15);
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::setUniforms(Shader& shader, const std::string& prefix)
{
	BaseLight::setUniforms(shader, prefix + ".base");
	shader.setUniform3(prefix + ".direction", m_Direction);
}