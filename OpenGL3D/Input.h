#pragma once

#include <map>

class Input
{
public:
	static void pressKey(int key);
	static void releaseKey(int key);
	static void pressMouse(int button);
	static void releaseMouse(int button);
	static void setMousePosition(int x, int y);
	static void setMouseMotionDelta(int x, int y);
	static void scrollMouseWheel(int x, int y);
	static void update();

	static bool isKeyJustPressed(int key);
	static bool isKeyDown(int key);
	static bool isMouseJustPressed(int button);
	static bool isMouseDown(int button);
	static int getX() { return mouseX; }
	static int getY() { return mouseY; }
	static int getDeltaX() { return relativeMouseX; }
	static int getDeltaY() { return relativeMouseY; }
	static int getMouseWheelHorizontal() { return mouseWheelX; }
	static int getMouseWheelVertical() { return mouseWheelY; }

private:
	static std::map<int, int> m_KeyMap;
	static std::map<int, int> mouseMap;
	static int mouseX, mouseY;
	static int relativeMouseX, relativeMouseY;
	static int mouseWheelX, mouseWheelY;
};