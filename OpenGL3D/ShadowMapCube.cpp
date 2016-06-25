#include "ShadowMapCube.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

glm::mat4 ShadowMapCube::s_Projection;

ShadowMapCube::ShadowMapCube()
{

}

ShadowMapCube::~ShadowMapCube()
{
	if (m_Fbo)
		glDeleteFramebuffers(1, &m_Fbo);
	if (m_Depth)
		glDeleteTextures(1, &m_Depth);
	if (m_ShadowMap)
		glDeleteTextures(1, &m_ShadowMap);
}

void ShadowMapCube::initProjections(unsigned int width, unsigned int height)
{
	s_Projection = glm::perspective(glm::radians<float>(90.0f), (float)width / (float)height, 0.1f, 1000.0f);
}

bool ShadowMapCube::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	m_Width = WindowWidth;
	m_Height = WindowHeight;

	//create the frame buffer
	glGenFramebuffers(1, &m_Fbo);

	GLenum error = glGetError();
	if (error)
		std::cout << "1: " << error << std::endl;
	
	//create the depth buffer
	glGenTextures(1, &m_Depth);
	glBindTexture(GL_TEXTURE_2D, m_Depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (error = glGetError())
		std::cout << "2: " << error << std::endl;

	glGenTextures(1, &m_ShadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (error = glGetError())
		std::cout << "3: " << error << std::endl;

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, WindowWidth, WindowHeight, 0, GL_RED, GL_FLOAT, NULL);
		if (error = glGetError())
			std::cout << "4: (" << i << ") " << error << std::endl;
	}
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	if (error = glGetError())
		std::cout << "5: " << error << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Depth, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Camera cameras[6];// =
	/*{
		Camera(glm::vec3(0, 0, 0), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		Camera(glm::vec3(0, 0, 0), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		Camera(glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		Camera(glm::vec3(0, 0, 0), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		Camera(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		Camera(glm::vec3(0, 0, 0), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};*/

	for (int i = 0; i < 6; i++)
	{
		CameraDirection cd;
		cd.cubemapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		cd.camera = cameras[i];
		cd.test = i + 1;
		m_Faces.push_back(cd);
	}

	if (error = glGetError())
		std::cout << "6: " << error << std::endl;

	return true;
}

void ShadowMapCube::BindForWriting(GLenum cubeFace)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubeFace, m_ShadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void ShadowMapCube::BindForReading(int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);
}

void ShadowMapCube::setLightPosition(const glm::vec3& position)
{
	//for (auto& it : m_Faces)
		//it.camera.setPosition(position);
}