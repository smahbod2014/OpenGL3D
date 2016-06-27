#pragma once

#include <GL/glew.h>

class ShadowMapFramebuffer
{
public:
	ShadowMapFramebuffer(int width, int height);
	~ShadowMapFramebuffer();

	void bindFramebuffer();
	void unbindFramebuffer();
	void initializeFrameBuffer();
	void bindFramebuffer(GLuint buffer, int width, int height);
	GLuint createFramebuffer();
	GLuint createDepthBufferAttachment(int width, int height);
	
	GLuint getShadowMapTexture() { return shadowMapTexture; }

private:
	int width, height;
	GLuint fbo, shadowMapTexture;
};

