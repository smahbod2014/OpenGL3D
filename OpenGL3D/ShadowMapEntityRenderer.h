#pragma once

#include <glm/glm.hpp>
#include <vector>

class Shader;
class Entity;

class ShadowMapEntityRenderer
{
public:
	ShadowMapEntityRenderer(Shader* shader, glm::mat4* projectionViewMatrix);
	~ShadowMapEntityRenderer();

	void render(const std::vector<Entity*> entities);

private:
	glm::mat4* projectionViewMatrix;
	Shader* shader;
};

