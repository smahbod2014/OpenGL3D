#include "ShadowBuffer.h"
#include "Window.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

ShadowBuffer::ShadowBuffer(int width, int height)
{
	m_Width = width;
	m_Height = height;

	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

	glGenTextures(1, &m_TexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TexID, 0);

	glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer incomplete" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowBuffer::~ShadowBuffer()
{
	if (m_Fbo)
		glDeleteFramebuffers(1, &m_Fbo);
	if (m_TexID)
		glDeleteTextures(1, &m_TexID);
}

void ShadowBuffer::setUniforms(Shader& shadowShader, Shader& regularShader, Light& light)
{
	glm::vec3 lightInvDir = glm::vec3(0.5f, 2.0f, 2.0f);

	Camera camera(glm::vec3(-5, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glm::mat4 depthViewMatrix = camera.getInverseViewMatrix();
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	glm::mat4 depthBiasMatrix = biasMatrix * depthMVP;

	shadowShader.bind();
	shadowShader.setUniformMatrix4("depthMVP", depthMVP);

	regularShader.bind();
	regularShader.setUniformMatrix4("depthBiasMVP", depthBiasMatrix);
	regularShader.setUniform1("sampler", 0);
	regularShader.setUniform1("shadowMap", 1);
	regularShader.unbind();
}

void ShadowBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glViewport(0, 0, m_Width, m_Height);
}

void ShadowBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::getWidth(), Window::getHeight());
}