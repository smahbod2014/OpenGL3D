#include "Camera.h"
#include "Input.h"
#include "Constants.h"
#include "GLMHelper.h"
#include "Helpers.h"
#include <SDL/SDL_keycode.h>
#include <glm/gtx/transform.hpp>
#include <iostream>

#ifdef USE_OLD_CAMERA
Camera::Camera()
{
	set(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
	m_Forward = glm::normalize(m_Position - m_LookAt);
	m_Right = glm::normalize(glm::cross(m_WorldUp, m_Forward));
	m_Up = glm::cross(m_Forward, m_Right);
	m_Up = glm::normalize(m_Up);

	m_ViewMatrix = glm::mat4(glm::vec4(m_Right, 0.0f),
							 glm::vec4(m_Up, 0.0f),
							 glm::vec4(m_Forward, 0.0f),
							 glm::vec4(m_Position, 1.0f));

	//m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
	m_InverseViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0, 1, 0));
	//m_InverseViewMatrix = createViewMatrix(this);

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

	if (Input::getMouseWheelVertical() != 0) {
		translate(m_Forward * (float)Input::getMouseWheelVertical() * 2.0f);
	}

	if (Input::isMouseDown(SDL_BUTTON_RIGHT)) {
		glm::vec3 newPos = getPosition() + -m_Right * (float)Input::getDeltaX() * 10.0f * dt + m_Up * (float)Input::getDeltaY() * 10.0f * dt;
		setPositionNoLook(newPos);
		m_LookAt = glm::vec3(0, 0, 0);
	}

	if (Input::isKeyJustPressed(SDLK_q)) {
		//pitch	
		printVec(-m_Forward);
	}

	if (Input::isKeyJustPressed(SDLK_e)) {
		//pitch	
		printVec(m_Right);
	}

	if (Input::isKeyJustPressed(SDLK_t)) {
		printVec(m_WorldUp);
	}

	if (Input::isKeyJustPressed(SDLK_f)) {
		printVec(m_Up);
	}

	if (Input::isKeyJustPressed(SDLK_r)) {
		float distance = 2.0f * (getPosition().y - 6);
		translate(glm::vec3(0, -distance, 0));
		invertPitch();
		std::cout << "-----------------" << std::endl;
		printVec(-m_Forward);
		printVec(m_Right);
		printVec(m_Up);
		//std::cout << *this << std::endl;
		std::cout << "-----------------" << std::endl;
		invertPitch();
		translate(glm::vec3(0, distance, 0));
	}
}

float Camera::getPitch()
{
	/*float numerator = glm::dot(m_Forward, glm::vec3(0, 1, 0));
	float denom1 = glm::length(glm::vec3(0, 1, 0));
	float denom2 = glm::length(m_Forward);
	float pitch = asinf(numerator / (denom1 * denom2));
	return rad2deg(pitch);*/
	float distance = m_LookAt.y - getPosition().y;
	glm::vec3 flatLookAt = m_LookAt;
	flatLookAt.y -= distance;
	glm::vec3 fakeForward = glm::normalize(m_Position - flatLookAt);
	float angle = acosf(glm::dot(-fakeForward, m_Forward));
	if (m_LookAt.y < m_Position.y)
		angle *= -1;
	return rad2deg(angle);
}

float Camera::getYaw()
{
	/*float numerator = glm::dot(m_Forward, glm::vec3(1, 0, 0));
	float denom1 = glm::length(glm::vec3(1, 0, 0));
	float denom2 = glm::length(m_Forward);
	float yaw = asinf(numerator / (denom1 * denom2));
	return rad2deg(yaw);*/

	/*glm::vec3 forward = m_Forward;
	forward.y = 0;
	forward = glm::normalize(forward);
	printVec(forward);
	float yaw = acosf(glm::dot(forward, glm::vec3(0, 0, -1)));
	yaw = rad2deg(yaw);
	if (forward.x < 0)
		yaw = 360 - yaw;
	return yaw;*/

	float distance = m_LookAt.x - getPosition().x;
	glm::vec3 flatLookAt = m_LookAt;
	flatLookAt.x -= distance;
	glm::vec3 fakeForward = glm::normalize(m_Position - flatLookAt);
	float angle = acosf(glm::dot(-fakeForward, m_Forward));
	if (m_LookAt.x < m_Position.x)
		angle *= -1;
	return rad2deg(angle);
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

void Camera::setPositionNoLook(const glm::vec3& position)
{
	m_Position = position;

	update();
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

std::ostream& operator<<(std::ostream& os, const Camera& camera)
{
	return os << "Camera position: " << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z;
}

void Camera::invertPitch()
{
	float distance = m_LookAt.y - getPosition().y;
	m_LookAt.y -= distance * 2;
	update();
	
	/*if (m_LookAt.y > getPosition().y) {
		float angle = acosf(glm::dot(glm::vec3(0, 0, -1), m_Forward)) * 180 / M_PI;
		rotate(m_Right, angle);
	}
	else {
		float angle = acosf(glm::dot(glm::vec3(0, 0, -1), m_Forward)) * 180 / M_PI;
		rotate(m_Right, -angle);
	}*/
}

#else

Camera::Camera()
{
	yaw = pitch = roll = 0;
}

Camera::~Camera() {}

void Camera::lookAt(const glm::vec3& target)
{
	//viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::input(float dt)
{
	float moveAmount = CAMERA_SPEED * dt;
	float rotateAmount = CAMERA_ROTATION * dt / 1.0f;

	if (Input::isKeyDown(SDLK_LEFT))
		yaw -= rotateAmount;
	if (Input::isKeyDown(SDLK_RIGHT))
		yaw += rotateAmount;
	if (Input::isKeyDown(SDLK_UP))
		pitch -= rotateAmount;
	if (Input::isKeyDown(SDLK_DOWN))
		pitch += rotateAmount;
	if (Input::isKeyDown(SDLK_w))
		translate(-forward * moveAmount);
	if (Input::isKeyDown(SDLK_s))
		translate(forward * moveAmount);
	if (Input::isKeyDown(SDLK_a))
		translate(-right * moveAmount);
	if (Input::isKeyDown(SDLK_d))
		translate(right * moveAmount);
	if (Input::isKeyDown(SDLK_v))
		translate(up * moveAmount);
	if (Input::isKeyDown(SDLK_x))
		translate(-up * moveAmount);
	

	if (Input::isKeyJustPressed(SDLK_q))
		printVec(forward);
	if (Input::isKeyJustPressed(SDLK_e))
		printVec(right);
	if (Input::isKeyJustPressed(SDLK_r))
		printVec(up);

	//std::cout << yaw << std::endl;

	update();
}

void Camera::translate(const glm::vec3 amount)
{
	position += amount;
	update();
}

void Camera::update()
{
	viewMatrix = createViewMatrix(this);

	float sx, sy, sz, cx, cy, cz, theta;

	// rotation angle about X-axis (pitch)
	theta = deg2rad(pitch);
	sx = sinf(theta);
	cx = cosf(theta);

	// rotation angle about Y-axis (yaw)
	theta = deg2rad(yaw);
	sy = sinf(theta);
	cy = cosf(theta);

	// rotation angle about Z-axis (roll)
	theta = deg2rad(roll);
	sz = sinf(theta);
	cz = cosf(theta);

	// determine left axis
	right.x = cy*cz;
	right.y = sx*sy*cz + cx*sz;
	right.z = -cx*sy*cz + sx*sz;

	right.z *= -1;
	//right.y *= -1;

	// determine up axis
	up.x = -cy*sz;
	up.y = -sx*sy*sz + cx*cz;
	up.z = cx*sy*sz + sx*cz;

	up.z *= -1;

	// determine forward axis
	forward.x = sy;
	forward.y = -sx*cy;
	forward.z = cx*cy;

	forward.x *= -1;
	forward.y *= -1;
}

#endif