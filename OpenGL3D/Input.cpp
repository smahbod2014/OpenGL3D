#include "Input.h"

std::map<int, int> Input::m_KeyMap;

void Input::pressKey(int key)
{
	auto it = m_KeyMap.find(key);
	
	if (it == m_KeyMap.end())
	{
		m_KeyMap[key] = 1;
	}
	else
	{
		if (m_KeyMap[key] != 2)
			m_KeyMap[key] = 1;
	}
}

void Input::releaseKey(int key)
{
	m_KeyMap[key] = 0;
}

void Input::update()
{
	for (auto& it : m_KeyMap)
	{
		if (it.second == 1)
			it.second = 2;
	}
}

bool Input::isKeyJustPressed(int key)
{
	auto it = m_KeyMap.find(key);

	if (it != m_KeyMap.end())
	{
		return m_KeyMap[key] == 1;
	}
	else
	{
		return false;
	}
}

bool Input::isKeyDown(int key)
{
	auto it = m_KeyMap.find(key);

	if (it != m_KeyMap.end())
	{
		return m_KeyMap[key] >= 1;
	}
	else
	{
		return false;
	}
}