#include "Transform.h"
#include <iostream>

Transform::Transform()
{
	m_Transform = glm::mat4(1.0);
}

Transform::Transform(const glm::mat4& matrix)
{
	m_Transform = matrix;
}

Transform::~Transform()
{

}

void Transform::draw(const glm::mat4& matrix)
{
	glm::mat4 combined = matrix * m_Transform;
	for (size_t i = 0; i < m_Children.size(); i++)
		m_Children[i]->draw(combined);
}

void Transform::translate(const glm::vec3& amount)
{
	m_Transform[3][0] += amount.x;
	m_Transform[3][1] += amount.y;
	m_Transform[3][2] += amount.z;
}

void Transform::setPosition(const glm::vec3& position)
{
	m_Transform[3][0] = position.x;
	m_Transform[3][1] = position.y;
	m_Transform[3][2] = position.z;
}

void Transform::operator*=(const glm::mat4& matrix)
{
	m_Transform *= matrix;
}

void Transform::operator%=(const glm::mat4& matrix)
{
	m_Transform = matrix * m_Transform;
}

glm::vec3 Transform::getTranslation() const
{
	glm::vec3 translation;
	translation.x = m_Transform[3][0];
	translation.y = m_Transform[3][1];
	translation.z = m_Transform[3][2];
	return translation;
}