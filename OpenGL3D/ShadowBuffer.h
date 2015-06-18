#pragma once

#include <GL/glew.h>

class Shader;
class Light;

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight, int numBuffers);

	void BindForWriting(int i);

	void BindForReading(int i);

private:
	GLuint* m_FBOs = nullptr;
	GLuint* m_ShadowMaps = nullptr;
	int m_NumBuffers;
};