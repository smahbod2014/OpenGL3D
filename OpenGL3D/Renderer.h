#pragma once

class Model;
class Shader;

class Renderer
{
	friend class Geode;
public:
	Renderer();
	~Renderer();

	void render(const Model& model);
	
	void setShader(Shader* shader) { m_Shader = shader; }
	void setShadowShader(Shader* shader) { m_ShadowShader = shader; }
private:
	Shader* m_Shader = nullptr;
	Shader* m_ShadowShader = nullptr;
};