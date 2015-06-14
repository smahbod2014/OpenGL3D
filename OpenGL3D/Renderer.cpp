#include "Renderer.h"
#include "Model.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::render(const Model& model)
{
	glBindVertexArray(model.m_Vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.m_Ibo);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, model.m_TexID);

	glDrawElements(GL_TRIANGLES, model.m_NumIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::shadowPass(const Model& model)
{
	glBindVertexArray(model.m_Vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.m_Ibo);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, model.m_TexID);

	glDrawElements(GL_TRIANGLES, model.m_NumIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}