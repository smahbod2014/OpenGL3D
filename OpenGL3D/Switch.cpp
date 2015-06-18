#include "Switch.h"

Switch::Switch(bool enabled /* = true */)
{
	m_Enabled = enabled;
}

Switch::~Switch()
{

}

void Switch::draw(const glm::mat4& matrix)
{
	if (!m_Enabled)
		return;
	Group::draw(matrix);
}