#include "Camera.h"
#include "Input.h"
#include "Constants.h"
#include <SDL/SDL_keycode.h>
#include <glm/gtx/transform.hpp>
#include <iostream>

Camera::Camera()
{
	set(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

Camera::Camera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& worldUp)
{
	set(position, lookAt, worldUp);
}

Camera::~Camera()
{

}

void Camera::update()
{
	m_Forward = glm::normalize((m_Position - m_LookAt));
	m_Right = glm::normalize(glm::cross(m_WorldUp, m_Forward));
	m_Up = glm::cross(m_Forward, m_Right);

	m_ViewMatrix = glm::mat4(glm::vec4(m_Right, 0.0f),
							 glm::vec4(m_Up, 0.0f),
							 glm::vec4(m_Forward, 0.0f),
							 glm::vec4(m_Position, 1.0f));

	m_InverseViewMatrix = glm::inverse(m_ViewMatrix);

	m_Forward *= -1.0f;
}

void Camera::input(float dt)
{
	float moveAmount = CAMERA_SPEED * dt;
	float rotateAmount = CAMERA_ROTATION * dt;

	

	if (Input::isKeyDown(SDLK_w))
		translate(m_Forward * moveAmount);
	if (Input::isKeyDown(SDLK_s))
		translate(m_Forward * -moveAmount);
	if (Input::isKeyDown(SDLK_a))
		translate(m_Right * -moveAmount);
	if (Input::isKeyDown(SDLK_d))
		translate(m_Right * moveAmount);
	if (Input::isKeyDown(SDLK_v))
		translate(m_Up * moveAmount);
	if (Input::isKeyDown(SDLK_x))
		translate(m_Up * -moveAmount);
	if (Input::isKeyDown(SDLK_LEFT))
		rotate(glm::vec3(0, 1, 0), rotateAmount);
	if (Input::isKeyDown(SDLK_RIGHT))
		rotate(glm::vec3(0, 1, 0), -rotateAmount);
	if (Input::isKeyDown(SDLK_UP))
		rotate(m_Right, rotateAmount);
	if (Input::isKeyDown(SDLK_DOWN))
		rotate(m_Right, -rotateAmount);
}

void Camera::translate(const glm::vec3& amount)
{
	m_Position += amount;
	m_LookAt += amount;
	update();
}

void Camera::setPosition(const glm::vec3& position)
{
	translate(position - m_Position);
}

void Camera::rotate(const glm::vec3& axis, float degrees)
{
	glm::mat4 rotation = glm::rotate(glm::radians<float>(degrees), axis);
	glm::mat4 translation = glm::translate(-m_Position);
	glm::mat4 translationInv = glm::translate(m_Position);

	glm::vec4 lookAtResult = translationInv * rotation * translation * glm::vec4(m_LookAt, 1.0f);
	glm::vec4 worldUpResult = rotation * glm::vec4(m_Up, 0.0f);

	m_LookAt = glm::vec3(lookAtResult);
	m_WorldUp = glm::vec3(worldUpResult);

	update();
}

void Camera::set(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& worldUp)
{
	m_Position = position;
	m_LookAt = lookAt;
	m_WorldUp = worldUp;
	update();
}