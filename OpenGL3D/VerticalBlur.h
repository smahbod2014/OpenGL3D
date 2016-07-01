#pragma once

#include <GL/glew.h>

class ImageRenderer;
class Shader;

class VerticalBlur
{
public:
	VerticalBlur(int fboWidth, int fboHeight);
	~VerticalBlur();

	GLuint render(GLuint targetTexture);
	GLuint getOutputTexture();
private:
	ImageRenderer* renderer;
	Shader* shader;
};

