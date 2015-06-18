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
private:
	GLuint m_Fbo;
	GLuint m_ShadowMap;
	GLuint m_Depth;
public:
	static glm::mat4 s_Projection;
	std::vector<CameraDirection> m_Faces;
};