#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

class Window
{
public:
	Window(const std::string& name, int width, int height);
	~Window();

	void begin();
	void swapBuffer();
	void end();
	void calculateFPS();
	
	void setMaxFPS(float fps) { m_MaxFps = fps; }
	bool shouldClose() const { return m_ShouldClose; }
	int getTicks() const { return m_Ticks; }
	float getFps() const { return m_InstantaneousFps; }
	float getDelta() const { return m_Delta; }

	static const glm::vec4& getClearColor() { return m_ClearColor; }
	static int getWidth() { return m_Width; }
	static int getHeight() { return m_Height; }
private:
	static int m_Width, m_Height;
	static glm::vec4 m_ClearColor;
	
	SDL_Window* m_Window;
	std::string m_Name;
	bool m_ShouldClose = false;
	Uint32 m_StartFrameTime = 0;
	int m_Ticks = 0;
	float m_Fps = 0;
	float m_MaxFps = 60.0f;
	float m_InstantaneousFps = 60.0f;
	float m_Delta = 0;
};