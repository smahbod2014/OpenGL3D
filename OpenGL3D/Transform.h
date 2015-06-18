#pragma once

#include "Group.h"
#include <vector>

class Transform : public Group
{
public:
	Transform();
	Transform(const glm::mat4& matrix);
	virtual ~Transform();

	virtual void draw(const glm::mat4& matrix) override;

	void translate(const glm::vec3& amount);
	void setPosition(const glm::vec3& position);

	void operator*=(const glm::mat4& matrix);
	void operator%=(const glm::mat4& matrix);

	glm::vec3 getTranslation() const;
private:
	glm::mat4 m_Transform;
};