#pragma once

#include <glm/glm.hpp>

enum class LightType { DIRECTIONAL = 0, POINT = 1 };

class Shader;

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& color, LightType type);
	~Light();

	void setAttenuation(float constant, float linear, float quadratic);
	void setUniforms(Shader& shader);
	void translate(const glm::vec3& amount);

	const glm::vec3& getPosition() const { return m_Position; }
	const glm::vec3& getColor() const { return m_Color; }
	const LightType& getType() const { return m_Type; }
	float getConstant() const { return m_Constant; }
	float getLinear() const { return m_Linear; }
	float getQuadratic() const { return m_Quadratic; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Color;
	LightType m_Type;
	float m_Constant = 1, m_Linear = 0, m_Quadratic = 0;
};