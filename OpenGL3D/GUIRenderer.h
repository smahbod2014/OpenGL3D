#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Model;
class Shader;

struct GUITexture
{
	GLuint texID;
	glm::vec2 position;
	glm::vec2 scale;

	GUITexture(GLuint texID, const glm::vec2& position, const glm::vec2& scale)
		: texID(texID), position(position), scale(scale) {}
};

class GUIRenderer
{
public:
	GUIRenderer();
	~GUIRenderer();

	void render(GUITexture* guiTexture);

private:
	Model* quad;
	Shader* shader;
};

