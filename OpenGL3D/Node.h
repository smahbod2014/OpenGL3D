#pragma once

#include <glm/glm.hpp>

class Node
{
public:
	Node();
	virtual ~Node();

	virtual void draw(const glm::mat4& matrix) = 0;

	void setRenderPassIndex(int i) { m_RenderPassIndex = i; }
protected:
	static int m_RenderPassIndex;
};