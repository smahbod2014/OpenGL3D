#pragma once

#include <GL/glew.h>

class ImageRenderer;
class Shader;

class HorizontalBlur
{
public:
	HorizontalBlur(int fboWidth, int fboHeight);
	~HorizontalBlur();

	GLuint render(GLuint targetTexture);
	GLuint getOutputTexture();
private:
	ImageRenderer* renderer;
	Shader* shader;
};

