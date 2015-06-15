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
	shader.setUniform3("lightDirection", m_Direction);
	shader.setUniform1("lightType", (int)m_Type);
	shader.setUniform1("lightConstant", m_Constant);
	shader.setUniform1("lightLinear", m_Linear);
	shader.setUniform1("lightQuadratic", m_Quadratic);
	shader.setUniform1("lightCutoff", m_AdjustedCutoff);
}

void Light::setDirection(const glm::vec3& direction)
{
	m_Direction = glm::normalize(direction);
}

void Light::setCutoff(float degrees)
{
	m_Cutoff = degrees;
	m_AdjustedCutoff = cosf(glm::radians<float>(degrees));
}

void Light::translate(const glm::vec3& amount)
{
	m_Position += amount;
}