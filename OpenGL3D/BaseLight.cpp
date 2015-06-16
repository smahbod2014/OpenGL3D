#include "BaseLight.h"
#include "Shader.h"

BaseLight::BaseLight(const glm::vec3& color, bool isActive /* = true */)
{
	m_Color = color;
	m_IsActive = isActive;
}

BaseLight::BaseLight(unsigned int color, bool isActive /* = true */)
{
	float r = ((color >> 24) & 0xff) / 255.0f;
	float g = ((color >> 16) & 0xff) / 255.0f;
	float b = ((color >> 8) & 0xff) / 255.0f;
	m_Color = glm::vec3(r, g, b);
	m_IsActive = isActive;
}

BaseLight::~BaseLight()
{

}

void BaseLight::setUniforms(Shader& shader, const std::string& prefix)
{
	shader.setUniform1(prefix + ".isActive", (int)m_IsActive);
	shader.setUniform3(prefix + ".color", m_Color);
}