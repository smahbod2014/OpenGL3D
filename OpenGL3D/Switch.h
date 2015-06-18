#pragma once

#include "Group.h"

class Switch : public Group
{
public:
	Switch(bool enabled = true);
	virtual ~Switch();

	virtual void draw(const glm::mat4& matrix) override;

	void setEnabled(bool enabled) { m_Enabled = enabled; }

	bool isEnabled() const { return m_Enabled; }
protected:
	bool m_Enabled;
};