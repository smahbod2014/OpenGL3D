#include "ImageRenderer.h"
#include "Fbo.h"

ImageRenderer::ImageRenderer(int width, int height)
{
	fbo = new Fbo(width, height, DepthBufferType::NONE);
}

ImageRenderer::ImageRenderer()
{
	
}

ImageRenderer::~ImageRenderer()
{
	if (fbo) delete fbo;
}

void ImageRenderer::renderQuad()
{
	if (fbo)
		fbo->bindFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo)
		fbo->unbindFramebuffer();
}

GLuint ImageRenderer::getOutputTexture()
{
	return fbo->getColorTexture();
}