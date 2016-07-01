#include "SkyboxRenderer.h"
#include "Model.h"
#include "TextureManager.h"
#include "Shader.h"
#include "Helpers.h"
#include "Camera.h"
#include "Window.h"

SkyboxRenderer::SkyboxRenderer(const std::string& alias, float size, float rotationSpeed)
{
	cube = new Model();
	
	float vertices[] = {
		-size, size, -size,
		-size, -size, -size,
		size, -size, -size,
		size, -size, -size,
		size, size, -size,
		-size, size, -size,

		-size, -size, size,
		-size, -size, -size,
		-size, size, -size,
		-size, size, -size,
		-size, size, size,
		-size, -size, size,

		size, -size, -size,
		size, -size, size,
		size, size, size,
		size, size, size,
		size, size, -size,
		size, -size, -size,

		-size, -size, size,
		-size, size, size,
		size, size, size,
		size, size, size,
		size, -size, size,
		-size, -size, size,

		-size, size, -size,
		size, size, -size,
		size, size, size,
		size, size, size,
		-size, size, size,
		-size, size, -size,

		-size, -size, -size,
		-size, -size, size,
		size, -size, -size,
		size, -size, -size,
		-size, -size, size,
		size, -size, size
	};

	cube->generateVerticesOnly(vertices, 36);
	texID = TextureManager::getTexture(alias);
	shader = Shader::createSkyboxDefault();
	shader->bind();
	shader->setUniformMatrix4("P", getDefaultProjectionMatrix());
	shader->setUniform1("cubeMap", 0);
	shader->unbind();

	this->rotationSpeed = rotationSpeed;
	currentRotation = 0;
}


SkyboxRenderer::~SkyboxRenderer()
{
}

void SkyboxRenderer::render(Camera* camera)
{
	shader->bind();
	glm::mat4 V = camera->getInverseViewMatrix();
	V[3][0] = 0.0f;
	V[3][1] = 0.0f;
	V[3][2] = 0.0f;
	currentRotation += rotationSpeed * Window::getDelta();
	V = V * glm::rotate(glm::radians(currentRotation), glm::vec3(0.0f, 1.0f, 0.0f));
	shader->setUniformMatrix4("V", V);
	glBindVertexArray(cube->m_Vao);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->unbind();
}