#pragma once

#include <map>

class Input
{
public:
	static void pressKey(int key);
	static void releaseKey(int key);
	static void update();

	static bool isKeyJustPressed(int key);
	static bool isKeyDown(int key);

private:
	static std::map<int, int> m_KeyMap;
};