#pragma once

#include <glm/glm.hpp>

class Model;
class Shader;
class Camera;
class DirectionalLight;

class Renderer
{
	friend class Geode;
public:
	Renderer();
	~Renderer();

	void render(const Model& model);
	
	void setShader(Shader* shader) { m_Shader = shader; }
	void setShadowShader(Shader* shader) { m_ShadowShader = shader; }
	void setCamera(Camera* camera);
	void updateCamera();
	void loadDirectionalLight(DirectionalLight* dLight);
private:
	Shader* m_Shader = nullptr;
	Shader* m_ShadowShader = nullptr;
	Camera* m_Camera;
	glm::mat4 P;
};