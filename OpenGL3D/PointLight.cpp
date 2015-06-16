#include "PointLight.h"
#include "Shader.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float ca, float la, float qa) :
	BaseLight(color)
{
	m_Position = position;
	m_Constant = ca;
	m_Linear = la;
	m_Quadratic = qa;
}

PointLight::PointLight(const glm::vec3& position, unsigned int color, float ca, float la, float qa) :
	BaseLight(color)
{
	m_Position = position;
	m_Constant = ca;
	m_Linear = la;
	m_Quadratic = qa;
}

PointLight::~PointLight()
{

}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
	m_Constant = constant;
	m_Linear = linear;
	m_Quadratic = quadratic;
}

void PointLight::translate(const glm::vec3& amount)
{
	m_Position += amount;
}

void PointLight::setPosition(const glm::vec3& position)
{
	m_Position = position;
}

void PointLight::setUniforms(Shader& shader, const std::string& prefix)
{
	BaseLight::setUniforms(shader, prefix + ".base");
	shader.setUniform3(prefix + ".position", m_Position);
	shader.setUniform1(prefix + ".attenuation.constantAtten", m_Constant);
	shader.setUniform1(prefix + ".attenuation.linearAtten", m_Linear);
	shader.setUniform1(prefix + ".attenuation.quadraticAtten", m_Quadratic);
}