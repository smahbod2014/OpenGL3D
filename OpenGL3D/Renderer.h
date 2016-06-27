#pragma once

#include <glm/glm.hpp>
#include <vector>

class Model;
class Shader;
class Camera;
class DirectionalLight;
class Entity;

class Renderer
{
	friend class Geode;
public:
	Renderer();
	~Renderer();

	void render(const Model& model, Camera* camera);
	void render(std::vector<Entity*> entities, Camera* camera);
	
	void setShader(Shader* shader) { m_Shader = shader; }
	void setShadowShader(Shader* shader) { m_ShadowShader = shader; }
	void loadDirectionalLight(DirectionalLight* dLight);
	void loadClipPlane(float x, float y, float z, float w);
private:
	Shader* m_Shader = nullptr;
	Shader* m_ShadowShader = nullptr;
};