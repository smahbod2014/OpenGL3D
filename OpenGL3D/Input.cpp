#include "Input.h"

std::map<int, int> Input::m_KeyMap;
std::map<int, int> Input::mouseMap;
int Input::mouseX = 0;
int Input::mouseY = 0;
int Input::relativeMouseX = 0;
int Input::relativeMouseY = 0;
int Input::mouseWheelX = 0;
int Input::mouseWheelY = 0;

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

void Input::pressMouse(int button)
{
	auto it = mouseMap.find(button);

	if (it == mouseMap.end())
	{
		mouseMap[button] = 1;
	}
	else
	{
		if (mouseMap[button] != 2)
			mouseMap[button] = 1;
	}
}

void Input::releaseMouse(int button)
{
	mouseMap[button] = 0;
}

void Input::setMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void Input::setMouseMotionDelta(int x, int y)
{
	relativeMouseX = x;
	relativeMouseY = y;
}

void Input::scrollMouseWheel(int x, int y)
{
	mouseWheelX = x;
	mouseWheelY = y;
}

void Input::update()
{
	for (auto& it : m_KeyMap)
	{
		if (it.second == 1)
			it.second = 2;
	}

	for (auto& it : mouseMap)
	{
		if (it.second == 1)
			it.second = 2;
	}

	relativeMouseX = 0;
	relativeMouseY = 0;
	mouseWheelX = 0;
	mouseWheelY = 0;
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

bool Input::isMouseJustPressed(int button)
{
	auto it = mouseMap.find(button);

	if (it != mouseMap.end())
	{
		return mouseMap[button] == 1;
	}
	else
	{
		return false;
	}
}

bool Input::isMouseDown(int button)
{
	auto it = mouseMap.find(button);

	if (it != mouseMap.end())
	{
		return mouseMap[button] >= 1;
	}
	else
	{
		return false;
	}
}