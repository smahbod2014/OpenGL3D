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

Model room;
Model quad;
ShadowMapFBO shadowMapFBO;
Camera camera;
Camera shadowCamera;
Shader shadowShader;
Shader regularShader;
Shader lightShader;
Renderer renderer;
Light spotLight(glm::vec3(0, 15, -15), glm::vec3(1, 1, 1), LightType::SPOT);

glm::mat4 objectTransformation;

void RenderPass();
void ShadowMapPass();


int main(int argc, char* argv[])
{
	float MAX_FPS = 120.0f;
	int width = 1440, height = 810;
	Window window("Open GL Testing", width, height);
	window.setMaxFPS(MAX_FPS);
	//Shader shader("Shaders/Shine.vert", "Shaders/Shine.frag");
	//Shader shadowShader("Shaders/Shadow.vert", "Shaders/Shadow.frag");
	//Light light(glm::vec3(0, -1, 0), glm::vec3(1, 1, 1), LightType::DIRECTIONAL);
	//Light light(glm::vec3(2, 7, -2), glm::vec3(1, 1, 1), LightType::POINT);
	
	//light.setAttenuation(1, 0, 0.0001f);
	
	assert(shadowMapFBO.Init(width, height));

	camera.set(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	room.generate("Models/room_thickwalls.obj");
	quad.generate(6.0f);
	shadowShader.load("Shaders/ShadowMap.vert", nullptr);
	//regularShader.load("Shaders/ShadowMap.vert", "Shaders/ShadowMap.frag");
	lightShader.load("Shaders/Shine.vert", "Shaders/Shine.frag");

	spotLight.setDirection(-spotLight.getPosition());
	spotLight.setCutoff(15.0f);
	spotLight.setAttenuation(1, 0, 0.0001f);

	lightShader.bind();
	lightShader.setUniformMatrix4("P", glm::perspective<float>(glm::radians<float>(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f));
	spotLight.setUniforms(lightShader);
	lightShader.setUniform1("sampler", 0);
	lightShader.setUniform1("gShadowMap", 1);
	lightShader.unbind();

	room.loadTexture("Textures/wtf face.png");


	float speed = 25.0f;
	float rotateSpeed = 40.0f;
	bool cameraOnLight = true;
	while (!window.shouldClose())
	{
		window.begin();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float dt = window.getDelta();

		camera.input(dt);
		objectTransformation *= glm::rotate(glm::radians<float>(rotateSpeed * dt), glm::vec3(0, 1, 0));

		glm::vec3 toOrigin = glm::normalize(glm::vec3(0, 0, 0) - spotLight.getPosition());
		if (Input::isKeyDown(SDLK_i))
			spotLight.translate(glm::vec3(0, 1, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_k))
			spotLight.translate(glm::vec3(0, -1, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_j))
			spotLight.translate(glm::vec3(1, 0, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_l))
			spotLight.translate(glm::vec3(-1, 0, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_y))
			spotLight.translate(toOrigin * speed * dt);
		if (Input::isKeyDown(SDLK_h))
			spotLight.translate(-toOrigin * speed * dt);
		if (Input::isKeyJustPressed(SDLK_p))
			cameraOnLight = !cameraOnLight;

		//point the spotlight towards the origin
		spotLight.setDirection(-spotLight.getPosition());

		if (cameraOnLight)
			camera.set(spotLight.getPosition(), spotLight.getDirection(), glm::vec3(0, 1, 0));

		ShadowMapPass();
		RenderPass();
		

		window.swapBuffer();
		window.end();
	}
	
	return 0;
}

void ShadowMapPass()
{
	//all that's left is to clean this code up and make it a bit more reusable!
	shadowMapFBO.BindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);

	shadowShader.bind();
	shadowCamera.set(spotLight.getPosition(), spotLight.getDirection(), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective<float>(glm::radians<float>(spotLight.getCutoff() * 2.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
	glm::mat4 WVP = proj * shadowCamera.getInverseViewMatrix() * objectTransformation;
	shadowShader.setUniformMatrix4("gWVP", WVP);
	renderer.render(room);
	shadowShader.unbind();

	lightShader.bind();
	lightShader.setUniformMatrix4("lightWVP", WVP);
	lightShader.unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadowMapFBO.BindForReading(1);
	room.bindTexture(0);
	lightShader.bind();
	lightShader.setUniformMatrix4("V", camera.getInverseViewMatrix());
	lightShader.setUniformMatrix4("M", objectTransformation);
	spotLight.setUniforms(lightShader);
	renderer.render(room);
	lightShader.unbind();
}