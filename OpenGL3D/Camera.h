#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& worldUp);
	~Camera();
	
	void update();
	void input(float dt);
	void translate(const glm::vec3& amount);
	void setPosition(const glm::vec3& position);
	void setPositionNoLook(const glm::vec3& position);
	void rotate(const glm::vec3& axis, float degrees);
	void set(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& worldUp);

	const glm::mat4& getInverseViewMatrix() const { return m_InverseViewMatrix; }
	const glm::vec3& getPosition() const { return m_Position; }
	const glm::vec3& getLookAt() const { return m_LookAt; }
	const glm::vec3& getUp() const { return m_Up; }
	const glm::vec3& getWorldUp() const { return m_WorldUp; }

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