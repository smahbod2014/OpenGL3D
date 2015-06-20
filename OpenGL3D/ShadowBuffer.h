#pragma once

#include <GL/glew.h>

class Shader;
class Light;

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting();

	void BindForReading(int i);

	unsigned int getWidth() const { return m_Width; }
	unsigned int getHeight() const { return m_Height; }
private:
	GLuint m_Fbo = 0;
	GLuint m_ShadowMap = 0;
	unsigned int m_Width, m_Height;
};