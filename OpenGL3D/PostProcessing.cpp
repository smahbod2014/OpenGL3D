#include "PostProcessing.h"
#include "Model.h"
#include "ContrastChanger.h"
#include "HorizontalBlur.h"
#include "VerticalBlur.h"
#include "Window.h"
#include "Shader.h"

Model* PostProcessing::quad = nullptr;
ContrastChanger* PostProcessing::contrastChanger = nullptr;
HorizontalBlur* PostProcessing::horizontalBlur = nullptr;
VerticalBlur* PostProcessing::verticalBlur = nullptr;
Shader* PostProcessing::postprocessingShader = nullptr;

PostProcessing::~PostProcessing()
{
	delete quad;
	delete contrastChanger;
	delete horizontalBlur;
	delete verticalBlur;
	delete postprocessingShader;
}

void PostProcessing::init()
{
	float positions[] = { -1, 1, -1, -1, 1, 1, 1, -1 };
	quad = new Model();
	quad->generate2dVerticesOnly(positions, 4);
	contrastChanger = new ContrastChanger();
	horizontalBlur = new HorizontalBlur(Window::getWidth()/8, Window::getHeight()/8);
	verticalBlur = new VerticalBlur(Window::getWidth()/8, Window::getHeight()/8);
	postprocessingShader = new Shader();
	postprocessingShader->load("Shaders/PostProcessing.vert", "Shaders/PostProcessing.frag");
	postprocessingShader->bind();
	postprocessingShader->setUniform1("renderTexture", 0);
	postprocessingShader->unbind();
}

void PostProcessing::start()
{
	//lazy initialization
	if (quad == nullptr)
		init();
	glBindVertexArray(quad->getVao());
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}

void PostProcessing::end(GLuint renderTexture)
{
	//now we render the quad to the screen using the default FBO
	postprocessingShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	postprocessingShader->unbind();

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessing::doPostProcessing(GLuint targetTexture)
{
	GLuint result = targetTexture;
	start();
	//stage 1: horizontal blur
	//result = horizontalBlur->render(result);
	//stage 2: vertical blur
	//result = verticalBlur->render(result);
	//stage 3: contrast change
	//result = contrastChanger->render(result);
	end(result);
}