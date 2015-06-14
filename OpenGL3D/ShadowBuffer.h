#pragma once

#include <GL/glew.h>

class Shader;
class Light;

class ShadowBuffer
{
public:
	ShadowBuffer(int width, int height);
	~ShadowBuffer();

	void setUniforms(Shader& shadowShader, Shader& regularShader, Light& light);
	void bind();
	void unbind();

	GLuint getTexID() const { return m_TexID; }

private:
	GLuint m_Fbo = 0;
	GLuint m_TexID = 0;
	int m_Width, m_Height;
};