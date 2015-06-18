#include "Group.h"

Group::Group()
{

}

Group::~Group()
{

}

void Group::addChild(Node* child)
{
	m_Children.push_back(child);
}

void Group::draw(const glm::mat4& matrix)
{
	for (auto& it : m_Children)
		it->draw(matrix);
}