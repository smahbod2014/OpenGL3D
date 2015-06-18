#pragma once

#include "Node.h"
#include <vector>

class Group : public Node
{
public:
	Group();
	virtual ~Group();
	
	virtual void draw(const glm::mat4& matrix) override;
	void addChild(Node* child);
protected:
	std::vector<Node*> m_Children;
};