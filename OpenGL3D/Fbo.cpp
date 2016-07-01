#include "Fbo.h"
#include "Window.h"

Fbo::Fbo(int width, int height, DepthBufferType type)
{
	this->width = width;
	this->height = height;
	initialize(type);
}

Fbo::~Fbo()
{
	if (frameBuffer) glDeleteFramebuffers(1, &frameBuffer);
	if (colorTexture) glDeleteTextures(1, &colorTexture);
	if (depthTexture) glDeleteTextures(1, &depthTexture);
	if (colorBuffer) glDeleteRenderbuffers(1, &colorBuffer);
	if (depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);
}

void Fbo::bindFramebuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Fbo::unbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::getWidth(), Window::getHeight());
}

void Fbo::bindForReading()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::initialize(DepthBufferType type)
{
	createFramebuffer();
	createTextureAttachment();
	if (type == DepthBufferType::DEPTH_RENDER_BUFFER)
		createDepthBufferAttachment();
	else if (type == DepthBufferType::DEPTH_TEXTURE)
		createDepthTextureAttachment();
	unbindFramebuffer();
}

void Fbo::createFramebuffer()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::createTextureAttachment()
{
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTexture, 0);
}

void Fbo::createDepthTextureAttachment()
{
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

void Fbo::createDepthBufferAttachment()
{
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}