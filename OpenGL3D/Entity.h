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
	void rotate(float axisx, float axisy, float axisz, float degrees);
	void scale(float x, float y, float z);
	void setNormalMap(const std::string& normalMapAlias);
	void setSpecularMap(const std::string& specularMapAlias);
	void setSpecularProperties(float damper, float reflectivity) { specularDamper = damper; specularReflectivity = reflectivity; }
	const glm::mat4& getTransformation() { return transformation; }
	Model* getModel() { return model; }
	GLuint getTexID() { return texID; }
	GLuint getNormalMapID() { return normalMapID; }
	GLuint getSpecularMapID () { return specularMapID; }
	float getSpecularDamper() { return specularDamper; }
	float getSpecularReflectivity() { return specularReflectivity; }
	
private:
	glm::mat4 transformation;
	GLuint texID = 0;
	GLuint normalMapID = 0;
	GLuint specularMapID = 0;
	Model* model;
	float specularDamper = 20.0f, specularReflectivity = 1.0f;
};

