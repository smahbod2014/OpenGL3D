#pragma once

#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "ShadowBuffer.h"
#include "ShadowMapCube.h"
#include "BaseLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Constants.h"
#include "ModelCache.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Geode.h"
#include "Switch.h"
#include "GLMHelper.h"
#include "Helpers.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class OpenGL3D
{
private:
	Window* m_Window;

public:
	OpenGL3D(const std::string& name, int width, int height)
	{
		m_Window = new Window(name, width, height);
		m_Window->setMaxFPS(120.0f);
	}

	virtual ~OpenGL3D()
	{
		if (m_Window)
			delete m_Window;
	}

	virtual void init() = 0;
	virtual void update(float dt) {}
	virtual void render() = 0;
	virtual void finish() {}

	void start()
	{
		init();
		while (!m_Window->shouldClose()) {
			m_Window->begin();
			m_Window->clearScreen();
			float dt = m_Window->getDelta();
			update(dt);
			render();
			m_Window->swapBuffer();
			m_Window->end();
		}
		finish();
	}

};