#include "GUIRenderer.h"
#include "Model.h"
#include "Shader.h"
#include "Helpers.h"

GUIRenderer::GUIRenderer()
{
	float positions[] = {
		-1, 1, -1, -1, 1, 1, 1, -1 };
	quad = new Model();
	quad->generate2dVerticesOnly(positions, 4);

	shader = new Shader();
	shader->load("Shaders/GUI.vert", "Shaders/GUI.frag");
}

GUIRenderer::~GUIRenderer()
{
}

void GUIRenderer::render(GUITexture* guiTexture)
{
	shader->bind();
	shader->setUniformMatrix4("M", createGUITransformation(guiTexture->position, guiTexture->scale));
	glBindVertexArray(quad->m_Vao);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, guiTexture->texID);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->unbind();
}