#include "VerticalBlur.h"
#include "Shader.h"
#include "ImageRenderer.h"
#include <iostream>

VerticalBlur::VerticalBlur(int fboWidth, int fboHeight)
{
	shader = new Shader();
	shader->load("Shaders/VerticalBlur.vert", "Shaders/GaussianBlur.frag");
	shader->bind();
	shader->setUniform1("originalTexture", 0);
	shader->setUniform1("fboHeight", (float)fboHeight);
	shader->unbind();
	renderer = new ImageRenderer(fboWidth, fboHeight);
}


VerticalBlur::~VerticalBlur()
{
	delete shader;
	delete renderer;
}

GLuint VerticalBlur::render(GLuint targetTexture)
{
	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, targetTexture);
	renderer->renderQuad();
	shader->unbind();
	return getOutputTexture();
}

GLuint VerticalBlur::getOutputTexture()
{
	return renderer->getOutputTexture();
}