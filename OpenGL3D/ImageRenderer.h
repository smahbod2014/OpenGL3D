#pragma once

#include <GL/glew.h>

class Fbo;

class ImageRenderer
{
public:
	ImageRenderer(int width, int height);
	ImageRenderer();
	~ImageRenderer();
	
	void renderQuad();
	GLuint getOutputTexture();

private:
	Fbo* fbo = nullptr;
};

