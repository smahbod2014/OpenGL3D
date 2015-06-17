#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "ShadowBuffer.h"
#include "BaseLight.h"
#include "SpotLight.h"
#include "Constants.h"
#include "ModelCache.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Geode.h"
#include "GLMHelper.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

Model room;
Model quad;
ShadowMapFBO shadowMapFBO;
Camera camera;
Shader shadowShader;
Shader lightShader;
Renderer renderer;
std::vector<BaseLight*> lights;

glm::mat4 objectTransformation;
glm::mat4 P;

Transform root;

void RenderPass();
void ShadowMapPass(int i);


int main(int argc, char* argv[])
{
	float MAX_FPS = 120.0f;
	int width = 1440, height = 810;
	Window window("Open GL Testing", width, height);
	window.setMaxFPS(MAX_FPS);

	ModelCache::loadModel("room", "Models/room_thickwalls.obj");
	ModelCache::loadModel("sphere", "Models/sphere2.obj");
	TextureManager::loadTexture("face", "Textures/wtf face.png");
	TextureManager::loadTexture("tiled", "Textures/tiled2.png");
	TextureManager::loadTexture("kalas", "Textures/whocares.png");

	camera.set(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//room.generate("Models/room_thickwalls.obj");
	
	const glm::vec3 spotPosition1(0, 15, -15);
	const glm::vec3 spotPosition2(10, 15, 0);
	SpotLight* spot1 = new SpotLight(spotPosition1, 0xdeff4cff, -spotPosition1, 10.0f, 1.0f, 0.0f, 0.0001f);
	SpotLight* spot2 = new SpotLight(spotPosition2, 0xb0ff75ff, -spotPosition2, 15.0f, 1.0f, 0.0f, 0.0001f);
	//lights.push_back(spot1);
	lights.push_back(spot2);
	assert(shadowMapFBO.Init(width, height, lights.size()));

	shadowShader.load("Shaders/ShadowMap.vert", nullptr);
	lightShader.load("Shaders/Shine.vert", "Shaders/Shine.frag");
	renderer.setShader(&lightShader);
	renderer.setShadowShader(&shadowShader);

	P = glm::perspective<float>(glm::radians<float>(60.0f), (float)width / (float)height, 0.1f, 1000.0f);

	lightShader.bind();
	lightShader.setUniform1("sampler", TEXTURE_INDEX);
	for (size_t i = 0; i < lights.size(); i++)
	{
		lights[i]->setUniforms(lightShader, "spotLights[" + std::to_string(i) + "]");
		lightShader.setUniform1("gShadowMap[" + std::to_string(i) + "]", SHADOW_BASE_INDEX + i);
	}
	lightShader.unbind();


	Transform* rotation = new Transform();
	Transform* upTranslation = new Transform(translation(3, 5, 0));
	Transform* upTranslation2 = new Transform(translation(-2, 8, -2));
	Geode* roomGeode = new Geode("room", &renderer);
	Geode* ballGeode = new Geode("sphere", &renderer);
	Geode* ballGeode2 = new Geode("sphere", &renderer);
	roomGeode->setTextureID(TextureManager::getTexture("face"));
	ballGeode->setTextureID(TextureManager::getTexture("tiled"));
	ballGeode2->setTextureID(TextureManager::getTexture("kalas"));
	root.addChild(rotation);
	rotation->addChild(roomGeode);
	rotation->addChild(upTranslation);
	rotation->addChild(upTranslation2);
	upTranslation->addChild(ballGeode);
	upTranslation2->addChild(ballGeode2);

	glm::vec3 velocity;
	glm::vec3 velocity2;
	glm::vec3 gravity(0, -60.0f, 0);


	float speed = 25.0f;
	float rotateSpeed = 30.0f;
	bool cameraOnLight = true;
	while (!window.shouldClose())
	{
		window.begin();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float dt = window.getDelta();

		camera.input(dt);
		//objectTransformation *= glm::rotate(glm::radians<float>(rotateSpeed * dt), glm::vec3(0, 1, 0));
		*rotation *= glm::rotate(glm::radians<float>(rotateSpeed * dt), glm::vec3(0, 1, 0));
		velocity += gravity * dt;
		velocity2 += gravity * dt;
		upTranslation->translate(velocity * dt);
		upTranslation2->translate(velocity2 * dt);

		if (upTranslation->getTranslation().y < 1.0f && velocity.y < 0)
		{
			velocity *= -1.0f;
			glm::vec3 currentPos = upTranslation->getTranslation();
			upTranslation->setPosition(glm::vec3(currentPos.x, 1.0f, currentPos.z));
		}

		if (upTranslation2->getTranslation().y < 1.0f && velocity2.y < 0)
		{
			velocity2 *= -1.0f;
			glm::vec3 currentPos = upTranslation2->getTranslation();
			upTranslation2->setPosition(glm::vec3(currentPos.x, 1.0f, currentPos.z));
		}

#if 0
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
#endif

		if (Input::isKeyJustPressed(SDLK_1))
			camera.set(((SpotLight*)lights[0])->getPosition(), ((SpotLight*)lights[0])->getDirection(), glm::vec3(0, 1, 0));
		if (Input::isKeyJustPressed(SDLK_2))
			camera.set(((SpotLight*)lights[1])->getPosition(), ((SpotLight*)lights[1])->getDirection(), glm::vec3(0, 1, 0));

		
		glCullFace(GL_FRONT);
		for (size_t i = 0; i < lights.size(); i++)
		{
			ShadowMapPass(i);
		}
		glCullFace(GL_BACK);
		RenderPass();

		window.swapBuffer();
		window.end();
	}
	
	return 0;
}

void ShadowMapPass(int i)
{
	//all that's left is to clean this code up and make it a bit more reusable!
	shadowMapFBO.BindForWriting(i);
	root.setRenderPassIndex(i);

	glClear(GL_DEPTH_BUFFER_BIT);

	SpotLight* sl = (SpotLight*)lights[i];

	shadowShader.bind();
	const glm::mat4& lightVP = sl->getProjection() * sl->getViewMatrix();
	shadowShader.setUniformMatrix4("lightVP", lightVP);
	root.draw(glm::mat4(1.0));
	shadowShader.unbind();


	//careful! this is still needed. or, move it to the render pass when updating the light uniforms?
	lightShader.bind();
	lightShader.setUniformMatrix4("lightVP[" + std::to_string(i) + "]", lightVP);
	lightShader.unbind();
}

void RenderPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	root.setRenderPassIndex(-1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < lights.size(); i++)
		shadowMapFBO.BindForReading(SHADOW_BASE_INDEX + i);

	lightShader.bind();
	const glm::mat4& VP = P * camera.getInverseViewMatrix();
	lightShader.setUniformMatrix4("VP", VP);

	//the lights can change. update them
	for (size_t i = 0; i < lights.size(); i++)
	{
		lights[i]->setUniforms(lightShader, "spotLights[" + std::to_string(i) + "]");
	}

	root.draw(glm::mat4(1.0));
	lightShader.unbind();
}