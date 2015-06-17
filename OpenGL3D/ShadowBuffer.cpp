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
	if (m_FBOs)
	{
		glDeleteFramebuffers(m_NumBuffers, m_FBOs);
		delete[] m_FBOs;
	}
	if (m_ShadowMaps)
	{
		glDeleteTextures(m_NumBuffers, m_ShadowMaps);
		delete[] m_ShadowMaps;
	}
}

bool ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight, int numBuffers)
{
	m_NumBuffers = numBuffers;
	m_FBOs = new GLuint[numBuffers];
	m_ShadowMaps = new GLuint[numBuffers];

	glGenFramebuffers(numBuffers, m_FBOs);
	glGenTextures(numBuffers, m_ShadowMaps);

	for (int i = 0; i < numBuffers; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_ShadowMaps[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMaps[i], 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) {
			printf("FB error, status: 0x%x\n", Status);
			return false;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMapFBO::BindForWriting(int i)
{
	assert(i < m_NumBuffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBOs[i]);
}

void ShadowMapFBO::BindForReading(int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMaps[i - SHADOW_BASE_INDEX]);
}