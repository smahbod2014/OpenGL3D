#include "ContrastChanger.h"
#include "Shader.h"
#include "ImageRenderer.h"
#include "Window.h"
#include <iostream>

ContrastChanger::ContrastChanger(int fboWidth, int fboHeight)
{
	shader = new Shader();
	shader->load("Shaders/Contrast.vert", "Shaders/Contrast.frag");
	shader->bind();
	shader->setUniform1("colorTexture", 0);
	shader->unbind();
	renderer = new ImageRenderer(Window::getWidth(), Window::getHeight());
	//renderer = new ImageRenderer();
}

ContrastChanger::~ContrastChanger()
{
	delete shader;
	delete renderer;
}

GLuint ContrastChanger::render(GLuint targetTexture)
{
	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, targetTexture);
	renderer->renderQuad();
	shader->unbind();
	return getOutputTexture();
}

GLuint ContrastChanger::getOutputTexture()
{
	return renderer->getOutputTexture();
}