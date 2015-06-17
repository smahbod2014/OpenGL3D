#pragma once

#include "Node.h"
#include <string>
#include <GL/glew.h>

class Model;
class Renderer;

class Geode : public Node
{
public:
	Geode(const std::string& alias, Renderer* renderer);
	virtual ~Geode();

	void draw(const glm::mat4& matrix) override;

	void setTextureID(GLuint texID) { m_TexID = texID; }
private:
	Model* m_Model;
	Renderer* m_Renderer;
	GLuint m_TexID = 0;
};