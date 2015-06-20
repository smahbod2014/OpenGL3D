#include "ShadowBuffer.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

ShadowMapFBO::ShadowMapFBO()
{
	
}

ShadowMapFBO::~ShadowMapFBO()
{
	if (m_Fbo)
		glDeleteFramebuffers(1, &m_Fbo);
	if (m_ShadowMap)
		glDeleteTextures(1, &m_ShadowMap);
}

bool ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	m_Width = WindowWidth;
	m_Height = WindowHeight;

	glGenFramebuffers(1, &m_Fbo);
	glGenTextures(1, &m_ShadowMap);

	GLenum error;

	if (error = glGetError())
		std::cout << "8: " << error << std::endl;

	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (error = glGetError())
		std::cout << "9: " << error << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);

	if (error = glGetError())
		std::cout << "10: " << error << std::endl;

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMapFBO::BindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
}

void ShadowMapFBO::BindForReading(int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
}