#pragma once

#include <GL/glew.h>

enum class DepthBufferType { NONE, DEPTH_RENDER_BUFFER, DEPTH_TEXTURE };

class Fbo
{
public:
	Fbo(int width, int height, int samples);
	Fbo(int width, int height, DepthBufferType type);
	~Fbo();

	void bindFramebuffer();
	void unbindFramebuffer();
	void bindForReading();
	void resolveToFbo(Fbo* outputFbo);
	void resolveToScreen();

	GLuint getColorTexture() { return colorTexture; }
	GLuint getDepthTexture() { return depthTexture; }

private:
	void initialize(DepthBufferType type);
	void createFramebuffer();
	void createTextureAttachment();
	void createDepthTextureAttachment();
	void createDepthBufferAttachment();
	void createMultisampleColorAttachment();
	
private:
	int width;
	int height;
	int samples = 0;

	GLuint frameBuffer = 0;
	GLuint colorTexture = 0;
	GLuint depthTexture = 0;
	GLuint depthBuffer = 0;
	GLuint colorBuffer = 0;
};

