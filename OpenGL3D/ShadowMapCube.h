#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"

class ShadowMapCube
{
public:
	struct CameraDirection
	{
		GLenum cubemapFace;
		Camera camera;
		int test;
	};

	ShadowMapCube();
	~ShadowMapCube();

	static void initProjections(unsigned int width, unsigned int height);

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting(GLenum cubeFace);

	void BindForReading(int i);

	void setLightPosition(const glm::vec3& position);

	unsigned int getWidth() const { return m_Width; }
	unsigned int getHeight() const { return m_Height; }
private:
	GLuint m_Fbo;
	GLuint m_ShadowMap;
	GLuint m_Depth;
	unsigned int m_Width, m_Height;
public:
	static glm::mat4 s_Projection;
	std::vector<CameraDirection> m_Faces;
};