#include "WaterFramebuffers.h"
#include "Window.h"

WaterFramebuffers::WaterFramebuffers()
{
	initializeReflectionFrameBuffer();
	initializeRefractionFrameBuffer();
}

WaterFramebuffers::~WaterFramebuffers()
{
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}

void WaterFramebuffers::bindReflectionFrameBuffer() {//call before rendering to this FBO
	bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
}

void WaterFramebuffers::bindRefractionFrameBuffer() {//call before rendering to this FBO
	bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
}

void WaterFramebuffers::unbindCurrentFrameBuffer() {//call to switch to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::getWidth(), Window::getHeight());
	glDisable(GL_CLIP_DISTANCE0);
}

void WaterFramebuffers::initializeReflectionFrameBuffer() {
	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void WaterFramebuffers::initializeRefractionFrameBuffer() {
	refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	unbindCurrentFrameBuffer();
}

void WaterFramebuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height){
	glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

GLuint WaterFramebuffers::createFrameBuffer() {
	GLuint buffer;
	glGenFramebuffers(1, &buffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	//create the framebuffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return buffer;
}

GLuint WaterFramebuffers::createTextureAttachment(int width, int height) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
					  0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							  texture, 0);
	return texture;
}

GLuint WaterFramebuffers::createDepthTextureAttachment(int width, int height){
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
					  0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							  texture, 0);
	return texture;
}

GLuint WaterFramebuffers::createDepthBufferAttachment(int width, int height) {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
							   height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
								   GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}