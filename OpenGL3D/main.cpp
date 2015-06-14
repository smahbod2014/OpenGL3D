#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "ShadowBuffer.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


int main(int argc, char* argv[])
{
	float MAX_FPS = 120.0f;
	Window window("OpenGL Testing", 960, 540);
	window.setMaxFPS(MAX_FPS);
	Shader shader("Shaders/Shine.vert", "Shaders/Shine.frag");
	Shader shadowShader("Shaders/Shadow.vert", "Shaders/Shadow.frag");
	Camera camera(glm::vec3(8, 12, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	Light light(glm::vec3(0, -1, 0), glm::vec3(1, 1, 1), LightType::DIRECTIONAL);
	//Light light(glm::vec3(2, 7, -2), glm::vec3(1, 1, 1), LightType::POINT);
	light.setAttenuation(1, 0, 0.0001f);
	Renderer renderer;
	ShadowBuffer shadowBuffer(1024, 1024);
	shadowBuffer.setUniforms(shadowShader, shader, light);

	
	
	//glm::mat4 proj = glm::ortho<float>(0, 32, 0, 18, -1, 1);
	glm::mat4 proj = glm::perspective<float>(glm::radians<float>(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	shader.bind();
	shader.setUniformMatrix4("P", proj);
	shader.unbind();

	glm::mat4 transformation(1.0);

	/*float vertices[] =
	{
		-3, 3, 0,
		-3, -3, 0,
		3, -3, 0,
		3, 3, 0
	};*/

	float baseSize = 10;
	float baseVertices[] =
	{
		-baseSize, 0, -baseSize,
		-baseSize, 0, baseSize,
		baseSize, 0, baseSize,
		baseSize, 0, -baseSize
	};

	float baseNormals[] =
	{
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	float planeSize = 3;
	float planeVertices[] =
	{
		-planeSize, 3, -planeSize,
		-planeSize, 3, planeSize,
		planeSize, 3, planeSize,
		planeSize, 3, -planeSize
	};

	float planeNormals[] =
	{
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	float texCoords[] =
	{
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	int indices[] = { 0, 1, 2, 2, 3, 0 };

	//std::cout << "Loading models" << std::endl;
	Model baseModel(baseVertices, texCoords, baseNormals, indices, 4, 6);
	baseModel.loadTexture("Textures/tiled2.png");
	Model planeModel(planeVertices, texCoords, planeNormals, indices, 4, 6);
	planeModel.loadTexture("Textures/wtf face.png");
	//std::cout << "Done loading models" << std::endl;

	Model room("Models/room_thickwalls.obj");
	room.loadTexture("Textures/wtf face.png");


	float t = 0;
	float speed = 40.0f;
	int dir = 1;
	while (!window.shouldClose())
	{
		window.begin();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float dt = window.getDelta();

		camera.input(dt);
		transformation *= glm::rotate(glm::radians<float>(speed * dt), glm::vec3(0, 1, 0));


		//-----First render pass
#if 1
		shadowBuffer.bind();
		shadowShader.bind();
		renderer.shadowPass(room);
		shadowBuffer.unbind();
#endif


#if 1
		//room
		shader.bind();
		shader.setUniformMatrix4("V", camera.getInverseViewMatrix());
		shader.setUniformMatrix4("M", glm::mat4(1.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, room.m_TexID);
		shader.setUniform1("sampler", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowBuffer.getTexID());
		shader.setUniform1("shadowMap", 1);
		light.setUniforms(shader);
		renderer.render(room);
		shader.unbind();
#endif

		window.swapBuffer();
		window.end();
	}
	
	return 0;
}