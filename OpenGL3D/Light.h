#pragma once

#include <glm/glm.hpp>

enum class LightType { DIRECTIONAL = 0, POINT = 1, SPOT = 2 };

class Shader;

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& color, LightType type);
	~Light();

	void setAttenuation(float constant, float linear, float quadratic);
	void setUniforms(Shader& shader);
	void setDirection(const glm::vec3& direction);
	void setCutoff(float degrees);
	void translate(const glm::vec3& amount);

	const glm::vec3& getPosition() const { return m_Position; }
	const glm::vec3& getColor() const { return m_Color; }
	const glm::vec3& getDirection() const { return m_Direction; }
	const LightType& getType() const { return m_Type; }
	float getConstant() const { return m_Constant; }
	float getLinear() const { return m_Linear; }
	float getQuadratic() const { return m_Quadratic; }
	float getCutoff() const { return m_Cutoff; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Color;
	glm::vec3 m_Direction;
	LightType m_Type;
	float m_Constant = 1, m_Linear = 0, m_Quadratic = 0;
	float m_Cutoff = 0, m_AdjustedCutoff = 0;
};