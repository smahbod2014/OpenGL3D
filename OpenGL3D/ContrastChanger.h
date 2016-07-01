#pragma once

#include <GL/glew.h>

class ImageRenderer;
class Shader;

class ContrastChanger
{
public:
	ContrastChanger(int fboWidth, int fboHeight);
	ContrastChanger() : ContrastChanger(0, 0) {}
	~ContrastChanger();

	GLuint render(GLuint targetTexture);
	GLuint getOutputTexture();

private:
	ImageRenderer* renderer;
	Shader* shader;
};

