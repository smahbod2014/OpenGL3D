#pragma once

#include <GL/glew.h>

class Model;
class ContrastChanger;
class HorizontalBlur;
class VerticalBlur;
class Shader;

class PostProcessing
{
private:
	PostProcessing() {}
	static void init();
	static void start();
	static void end(GLuint renderTexture);
public:
	~PostProcessing();

	static void doPostProcessing(GLuint targetTexture);
private:
	static Model* quad;
	static ContrastChanger* contrastChanger;
	static HorizontalBlur* horizontalBlur;
	static VerticalBlur* verticalBlur;
	static Shader* postprocessingShader;
	
};

