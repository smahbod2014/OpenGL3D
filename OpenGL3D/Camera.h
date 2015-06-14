#pragma once

#include <glm/glm.hpp>

#define CAMERA_SPEED 10.0f
#define CAMERA_ROTATION 120.0f

class Camera
{
public:
	Camera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& worldUp);
	~Camera();
	
	void update();
	void input(float dt);
	void translate(const glm::vec3& amount);
	void rotate(const glm::vec3& axis, float degrees);

	const glm::mat4& getInverseViewMatrix() const { return m_InverseViewMatrix; }
	const glm::vec3& getPosition() const { return m_Position; }

private:
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_InverseViewMatrix;
	
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_Position;
	glm::vec3 m_LookAt;
	glm::vec3 m_WorldUp;
};