#pragma once

class Model;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render(const Model& model);
	void shadowPass(const Model& model);
};