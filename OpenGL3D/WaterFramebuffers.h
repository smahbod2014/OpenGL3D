#pragma once

#include <GL/glew.h>

#define REFLECTION_WIDTH  1280
#define REFLECTION_HEIGHT 720

#define REFRACTION_WIDTH 1280
#define REFRACTION_HEIGHT 720

class WaterFramebuffers
{
public:
	WaterFramebuffers();
	~WaterFramebuffers();

	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();
	void unbindCurrentFrameBuffer();
	void initializeReflectionFrameBuffer();
	void initializeRefractionFrameBuffer();
	void bindFrameBuffer(GLuint frameBuffer, int width, int height);
	GLuint createFrameBuffer();
	GLuint createTextureAttachment(int width, int height);
	GLuint createDepthTextureAttachment(int width, int height);
	GLuint createDepthBufferAttachment(int width, int height);

public:
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;
	
	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;
};

