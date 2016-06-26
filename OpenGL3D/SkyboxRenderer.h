#pragma once

#include <GL/glew.h>
#include <string>

class Model;
class Shader;
class Camera;

class SkyboxRenderer
{
public:
	SkyboxRenderer(const std::string& alias) : SkyboxRenderer(alias, 500.0f, 0.0f) {}
	SkyboxRenderer(const std::string& alias, float size, float rotationSpeed);
	~SkyboxRenderer();

	void render(Camera* camera);
private:
	Model* cube;
	GLuint texID;
	Shader* shader;
	float rotationSpeed, currentRotation;
};

