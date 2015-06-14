#include "Light.h"
#include "Shader.h"

Light::Light(const glm::vec3& position, const glm::vec3& color, LightType type)
{
	m_Position = position;
	m_Color = color;
	m_Type = type;
}

Light::~Light()
{
	
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	m_Constant = constant;
	m_Linear = linear;
	m_Quadratic = quadratic;
}

void Light::setUniforms(Shader& shader)
{
	shader.setUniform3("lightPosition", m_Position);
	shader.setUniform3("lightColor", m_Color);
	shader.setUniform1("lightType", (int)m_Type);
	shader.setUniform1("lightConstant", m_Constant);
	shader.setUniform1("lightLinear", m_Linear);
	shader.setUniform1("lightQuadratic", m_Quadratic);
}

void Light::translate(const glm::vec3& amount)
{
	m_Position += amount;
}