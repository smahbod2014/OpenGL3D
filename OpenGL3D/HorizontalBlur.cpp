#include "HorizontalBlur.h"
#include "Shader.h"
#include "ImageRenderer.h"
#include <iostream>

HorizontalBlur::HorizontalBlur(int fboWidth, int fboHeight)
{
	shader = new Shader();
	shader->load("Shaders/HorizontalBlur.vert", "Shaders/GaussianBlur.frag");
	shader->bind();
	shader->setUniform1("originalTexture", 0);
	shader->setUniform1("fboWidth", (float)fboWidth);
	shader->unbind();
	renderer = new ImageRenderer(fboWidth, fboHeight);
}

HorizontalBlur::~HorizontalBlur()
{
	delete shader;
	delete renderer;
}

GLuint HorizontalBlur::render(GLuint targetTexture)
{
	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, targetTexture);
	renderer->renderQuad();
	shader->unbind();
	return getOutputTexture();
}

GLuint HorizontalBlur::getOutputTexture()
{
	return renderer->getOutputTexture();
}