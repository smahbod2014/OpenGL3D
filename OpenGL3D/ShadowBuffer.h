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

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};