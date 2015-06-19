#pragma once

#include "BaseLight.h"
#include "Camera.h"

class DirectionalLight : public BaseLight
{
public:
	DirectionalLight(const glm::vec3& direction, const glm::vec3& color);
	DirectionalLight(const glm::vec3& direction, unsigned int color);
	virtual ~DirectionalLight();

	virtual void setUniforms(Shader& shader, const std::string& prefix) override;

	const glm::vec3& getDirection() const { return m_Direction; }
	const glm::mat4& getViewMatrix() const { return m_Camera.getInverseViewMatrix(); }
	const glm::mat4& getProjection() const { return m_Projection; }
private:
	glm::vec3 m_Direction;
	Camera m_Camera;
	glm::mat4 m_Projection;
};