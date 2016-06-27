#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

class Model;

class Entity
{
public:
	Entity(const std::string& modelAlias, const std::string& textureAlias);
	~Entity();

	void translateLocal(float x, float y, float z);
	void translate(float x, float y, float z);

	const glm::mat4& getTransformation() { return transformation; }
	Model* getModel() { return model; }
	GLuint getTexID() { return texID; }
	
private:
	glm::mat4 transformation;
	GLuint texID = 0;
	Model* model;
};

