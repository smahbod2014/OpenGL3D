#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader;

class BaseLight
{
protected:
	BaseLight(const glm::vec3& color, bool isActive = true);
	BaseLight(unsigned int color, bool isActive = true);
public:
	virtual ~BaseLight();

	virtual void setUniforms(Shader& shader, const std::string& prefix);

	void setColor(const glm::vec3& color) { m_Color = color; }
	void setActive(bool active) { m_IsActive = active; }

	const glm::vec3& getColor() const { return m_Color; }
	bool isActive() const { return m_IsActive; }
protected:
	glm::vec3 m_Color;
	bool m_IsActive;
};