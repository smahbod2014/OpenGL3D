#pragma once

#include "Node.h"
#include <vector>

class Transform : public Node
{
public:
	Transform();
	Transform(const glm::mat4& matrix);
	virtual ~Transform();

	virtual void draw(const glm::mat4& matrix) override;
	void addChild(Node* child) { m_Children.push_back(child); }

	void translate(const glm::vec3& amount);
	void setPosition(const glm::vec3& position);

	void operator*=(const glm::mat4& matrix);
	void operator%=(const glm::mat4& matrix);

	glm::vec3 getTranslation() const;
private:
	glm::mat4 m_Transform;
	std::vector<Node*> m_Children;
};