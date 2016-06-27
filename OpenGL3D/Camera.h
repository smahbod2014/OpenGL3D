#pragma once

#include <glm/glm.hpp>
#include <iostream>

#define USE_OLD_CAMERA

#ifdef USE_OLD_CAMERA
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
	void invertPitch();

	const glm::mat4& getInverseViewMatrix() const { return m_InverseViewMatrix; }
	const glm::vec3& getPosition() const { return m_Position; }
	const glm::vec3& getLookAt() const { return m_LookAt; }
	const glm::vec3& getUp() const { return m_Up; }
	const glm::vec3& getWorldUp() const { return m_WorldUp; }
	const glm::vec3& getForward() const { return m_Forward; }
	float getPitch();
	float getYaw();

	friend std::ostream& operator<<(std::ostream& os, const Camera& camera);
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
#else
class Camera
{
public:
	Camera();
	~Camera();
	void lookAt(const glm::vec3& target);
	void input(float dt);
	void translate(const glm::vec3 amount);
	void update();
	void invertPitch() { pitch = -pitch; update(); }
	const glm::mat4& getInverseViewMatrix() const { return viewMatrix; }
	const glm::vec3& getPosition() const { return position; }
	float getPitch() { return pitch; }
	float getYaw() { return yaw; }
	float getRoll() { return roll; }
private:
	glm::mat4 viewMatrix;
	glm::vec3 position;
	float pitch, yaw, roll;
	glm::vec3 forward, right, up;
};
#endif