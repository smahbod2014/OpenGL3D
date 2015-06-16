#pragma once

#include "PointLight.h"
#include "Camera.h"
#include <glm/glm.hpp>

class Shader;

//the fact that I need this "declspec" is bullshit
__declspec(align(16)) class SpotLight : public PointLight
{
public:
	SpotLight(const glm::vec3& position, const glm::vec3& color, const glm::vec3& direction, float cutoff, float ca, float la, float qa);
	SpotLight(const glm::vec3& position, unsigned int color, const glm::vec3& direction, float cutoff, float ca, float la, float qa);
	virtual ~SpotLight();

	//these two operator overloads are bullshit, but I get a memory alignment warning otherwise...
	void* operator new(size_t i){ return _mm_malloc(i, 16); }
	void operator delete(void* p) { _mm_free(p); }

	virtual void setUniforms(Shader& shader, const std::string& prefix);

	void setDirection(const glm::vec3& direction);
	void setCutoff(float degrees);

	const glm::mat4& getProjection() const { return m_Projection; }
	const glm::mat4& getViewMatrix() const { return m_ShadowCamera.getInverseViewMatrix(); }
	const glm::vec3& getDirection() const { return m_Direction; }
	float getCutoff() const { return m_Cutoff; }
protected:
	glm::vec3 m_Direction;
	float m_Cutoff = 0, m_AdjustedCutoff = 0;
	Camera m_ShadowCamera;
	glm::mat4 m_Projection;
};