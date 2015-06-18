#pragma once

#include "BaseLight.h"
#include <glm/glm.hpp>
#include "Camera.h"

class PointLight : public BaseLight
{
public:
	PointLight(const glm::vec3& position, const glm::vec3& color, float ca, float la, float qa);
	PointLight(const glm::vec3& position, unsigned int color, float ca, float la, float qa);
	virtual ~PointLight();

	virtual void setUniforms(Shader& shader, const std::string& prefix) override;

	void setAttenuation(float constant, float linear, float quadratic);
	virtual void translate(const glm::vec3& amount);
	virtual void setPosition(const glm::vec3& position);

	const glm::vec3& getPosition() const { return m_Position; }
	
	float getConstant() const { return m_Constant; }
	float getLinear() const { return m_Linear; }
	float getQuadratic() const { return m_Quadratic; }
protected:
	glm::vec3 m_Position;
	float m_Constant, m_Linear, m_Quadratic;
};