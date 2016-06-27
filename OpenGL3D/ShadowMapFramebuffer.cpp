#include "ShadowMapFramebuffer.h"
#include "Window.h"

ShadowMapFramebuffer::ShadowMapFramebuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	initializeFrameBuffer();
}

ShadowMapFramebuffer::~ShadowMapFramebuffer()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &shadowMapTexture);
}

void ShadowMapFramebuffer::bindFramebuffer()
{
	bindFramebuffer(fbo, width, height);
}

void ShadowMapFramebuffer::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::getWidth(), Window::getHeight());
}

void ShadowMapFramebuffer::initializeFrameBuffer()
{
	fbo = createFramebuffer();
	shadowMapTexture = createDepthBufferAttachment(width, height);
	unbindFramebuffer();
}

void ShadowMapFramebuffer::bindFramebuffer(GLuint buffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);
	glViewport(0, 0, width, height);
}

GLuint ShadowMapFramebuffer::createFramebuffer()
{
	GLuint buffer;
	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	glDrawBuffer(GL_NONE);
	return buffer;
}

GLuint ShadowMapFramebuffer::createDepthBufferAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}