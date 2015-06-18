#include <iostream>
#include "Window.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "ShadowBuffer.h"
#include "ShadowMapCube.h"
#include "BaseLight.h"
#include "SpotLight.h"
#include "Constants.h"
#include "ModelCache.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Geode.h"
#include "Switch.h"
#include "GLMHelper.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

Model room;
Model quad;
ShadowMapFBO shadowMapFBO;
std::vector<ShadowMapCube*> pointLightFbos;
Camera camera;
Shader shadowShader;
Shader pointLightShadowShader;
Shader lightShader;
Renderer renderer;
std::vector<BaseLight*> lights;
std::vector<PointLight*> pointLights;

glm::mat4 P;

Transform root;
Switch wallSwitch;

void RenderPass();
void ShadowMapPass(int i);
void PointLightShadowMapPass(int i);


int main(int argc, char* argv[])
{
	float MAX_FPS = 120.0f;
	int width = 1440, height = 810;
	Window window("Open GL Testing", width, height);
	window.setMaxFPS(MAX_FPS);
	ShadowMapCube::initProjections(CUBE_MAP_DIMENSIONS, CUBE_MAP_DIMENSIONS);

	ModelCache::loadModel("plane", 30.0f);
	//ModelCache::loadModel("room", "Models/room_thickwalls.obj");
	ModelCache::loadModel("sphere", "Models/sphere2.obj");
	//TextureManager::loadTexture("face", "Textures/wtf face.png");
	TextureManager::loadTexture("tiled", "Textures/tiled2.png");
	TextureManager::loadTexture("kalas", "Textures/whocares.png");
	TextureManager::loadTexture("checker", "Textures/checkerboard.png");
	TextureManager::loadTexture("left", "Textures/test_left.png");
	TextureManager::loadTexture("bottom", "Textures/test_bottom.png");
	TextureManager::loadTexture("back", "Textures/test_back.png");
	TextureManager::loadTexture("right", "Textures/test_right.png");
	TextureManager::loadTexture("top", "Textures/test_top.png");
	TextureManager::loadTexture("front", "Textures/test_front.png");

	camera.set(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//room.generate("Models/room_thickwalls.obj");
	
	const glm::vec3 spotPosition1(15, 10, 2);
	const glm::vec3 spotPosition2(0, 15, 15);
	//SpotLight* spot1 = new SpotLight(spotPosition1, 0xdeff4cff, -spotPosition1, 15.0f, 1.0f, 0.0f, 0.0001f);
	SpotLight* spot2 = new SpotLight(glm::vec3(-3, 5, 3), 0xff3d57ff, glm::vec3(1, 0, -1), 30.0f, 1.0f, 0.0f, 0.0001f);
	//SpotLight* spot3 = new SpotLight(glm::vec3(10, 5, 0), 0xffffffff, glm::vec3(-1, 0, 0), 20.0f, 1.0f, 0.0f, 0.0001f);
	PointLight* point1 = new PointLight(glm::vec3(0, 8, 0), 0x66d8ffff, 1.0f, 0.0f, 0.0001f);
	//lights.push_back(spot2);
	pointLights.push_back(point1);

	if (lights.size() > 0)
		assert(shadowMapFBO.Init(width, height, lights.size()));

	for (auto& it : pointLights)
	{
		ShadowMapCube* pointLightFbo = new ShadowMapCube();
		assert(pointLightFbo->Init(CUBE_MAP_DIMENSIONS, CUBE_MAP_DIMENSIONS));
		pointLightFbos.push_back(pointLightFbo);
		std::cout << "Made an fbo" << std::endl;
	}

	shadowShader.load("Shaders/ShadowMap.vert", nullptr);
	lightShader.load("Shaders/Shine.vert", "Shaders/Shine.frag");
	pointLightShadowShader.load("Shaders/PointLightShadowMap.vert", "Shaders/PointLightShadowMap.frag");
	renderer.setShader(&lightShader);
	//renderer.setShadowShader(&pointLightShadowShader);

	P = glm::perspective<float>(glm::radians<float>(60.0f), (float)width / (float)height, 0.1f, 1000.0f);

	lightShader.bind();
	lightShader.setUniform1("sampler", TEXTURE_INDEX);
	for (size_t i = 0; i < lights.size(); i++)
	{
		lights[i]->setUniforms(lightShader, "spotLights[" + std::to_string(i) + "]");
		lightShader.setUniform1("gShadowMap[" + std::to_string(i) + "]", SHADOW_BASE_INDEX + i);
	}
	for (size_t i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->setUniforms(lightShader, "pointLights[" + std::to_string(i) + "]");
		lightShader.setUniform1("gShadowCubeMap[" + std::to_string(i) + "]", SHADOW_CUBE_BASE_INDEX + i);
	}
	lightShader.unbind();

	Transform* rotation = new Transform();
	Transform* upTranslation = new Transform(translation(0, 5, 0));
	Transform* upTranslation2 = new Transform(translation(-7.5f, 8, 0));
	Transform* wallRotation = new Transform(glm::rotate(glm::radians<float>(-90.0f), glm::vec3(0, 0, 1)));
	Transform* wallTranslation = new Transform(translation(glm::vec3(-15.0f, 15.0f, 0)));
	Transform* backWallRotation = new Transform(glm::rotate(glm::radians<float>(90.0f), glm::vec3(1, 0, 0)));
	Transform* backWallTranslation = new Transform(translation(glm::vec3(0, 15.0f, -15.0f)));
	Transform* rightWallRotation = new Transform(glm::rotate(glm::radians<float>(90.0f), glm::vec3(0, 0, 1)));
	Transform* rightWallTranslation = new Transform(translation(glm::vec3(15.0f, 15.0f, 0)));
	Transform* topWallRotation = new Transform(glm::rotate(glm::radians<float>(180), glm::vec3(1, 0, 0)));
	Transform* topWallTranslation = new Transform(translation(glm::vec3(0.0f, 30.0f, 0)));
	Transform* frontWallRotation = new Transform(glm::rotate(glm::radians<float>(-90.0f), glm::vec3(1, 0, 0)));
	Transform* frontWallTranslation = new Transform(translation(glm::vec3(0, 15.0f, 15.0f)));
	//Geode* roomGeode = new Geode("room", &renderer);
	Geode* floorGeode = new Geode("plane", &renderer);
	Geode* wallGeode = new Geode("plane", &renderer);
	Geode* rightWallGeode = new Geode("plane", &renderer);
	Geode* backWallGeode = new Geode("plane", &renderer);
	Geode* topWallGeode = new Geode("plane", &renderer);
	Geode* frontWallGeode = new Geode("plane", &renderer);
	Geode* ballGeode = new Geode("sphere", &renderer);
	Geode* ballGeode2 = new Geode("sphere", &renderer);
	//roomGeode->setTextureID(TextureManager::getTexture("face"));
	floorGeode->setTextureID(TextureManager::getTexture("bottom"));
	wallGeode->setTextureID(TextureManager::getTexture("left"));
	rightWallGeode->setTextureID(TextureManager::getTexture("right"));
	backWallGeode->setTextureID(TextureManager::getTexture("back"));
	topWallGeode->setTextureID(TextureManager::getTexture("top"));
	frontWallGeode->setTextureID(TextureManager::getTexture("front"));
	ballGeode->setTextureID(TextureManager::getTexture("tiled"));
	ballGeode2->setTextureID(TextureManager::getTexture("kalas"));
	root.addChild(rotation);
	rotation->addChild(&wallSwitch);
	wallSwitch.addChild(floorGeode);
	rotation->addChild(upTranslation);
	rotation->addChild(upTranslation2);
	upTranslation->addChild(ballGeode);
	upTranslation2->addChild(ballGeode2);
	wallSwitch.addChild(wallTranslation);
	wallTranslation->addChild(wallRotation);
	wallRotation->addChild(wallGeode);
	wallSwitch.addChild(rightWallTranslation);
	rightWallTranslation->addChild(rightWallRotation);
	rightWallRotation->addChild(rightWallGeode);
	wallSwitch.addChild(backWallTranslation);
	backWallTranslation->addChild(backWallRotation);
	backWallRotation->addChild(backWallGeode);
	wallSwitch.addChild(frontWallTranslation);
	frontWallTranslation->addChild(frontWallRotation);
	frontWallRotation->addChild(frontWallGeode);
	wallSwitch.addChild(topWallTranslation);
	topWallTranslation->addChild(topWallRotation);
	topWallRotation->addChild(topWallGeode);

	glm::vec3 velocity;
	glm::vec3 velocity2;
	glm::vec3 gravity(0, -60.0f, 0);


	float speed = 25.0f;
	float rotateSpeed = -7.5f;
	bool cameraOnLight = false;
	while (!window.shouldClose())
	{
		window.begin();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float dt = window.getDelta();

		camera.input(dt);
		
		//*rotation *= glm::rotate(glm::radians<float>(rotateSpeed * dt), glm::vec3(0, 1, 0));
		if (glGetError())
			std::cout << "7: " << glGetError() << std::endl;
#if 1
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
#endif

#if 1
		PointLight* pl = pointLights[0];
		//glm::vec3 toOrigin = glm::normalize(glm::vec3(0, 0, 0) - sl->getPosition());
		if (Input::isKeyDown(SDLK_i))
			pl->translate(glm::vec3(0, 1, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_k))
			pl->translate(glm::vec3(0, -1, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_j))
			pl->translate(glm::vec3(-1, 0, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_l))
			pl->translate(glm::vec3(1, 0, 0) * speed * dt);
		if (Input::isKeyDown(SDLK_p))
			pl->translate(glm::vec3(0, 0, -1) * speed * dt);
		if (Input::isKeyDown(SDLK_SEMICOLON))
			pl->translate(glm::vec3(0, 0, 1) * speed * dt);
#endif
		
		if (Input::isKeyJustPressed(SDLK_1))
			camera.set(pointLightFbos[0]->m_Faces[0].camera.getPosition(), pointLightFbos[0]->m_Faces[0].camera.getLookAt(), pointLightFbos[0]->m_Faces[0].camera.getUp());
		if (Input::isKeyJustPressed(SDLK_2))
			camera.set(pointLightFbos[0]->m_Faces[1].camera.getPosition(), pointLightFbos[0]->m_Faces[1].camera.getLookAt(), pointLightFbos[0]->m_Faces[1].camera.getUp());
		if (Input::isKeyJustPressed(SDLK_3))
			camera.set(pointLightFbos[0]->m_Faces[2].camera.getPosition(), pointLightFbos[0]->m_Faces[2].camera.getLookAt(), pointLightFbos[0]->m_Faces[2].camera.getUp());
		if (Input::isKeyJustPressed(SDLK_4))
			camera.set(pointLightFbos[0]->m_Faces[3].camera.getPosition(), pointLightFbos[0]->m_Faces[3].camera.getLookAt(), pointLightFbos[0]->m_Faces[3].camera.getUp());
		if (Input::isKeyJustPressed(SDLK_5))
			camera.set(pointLightFbos[0]->m_Faces[4].camera.getPosition(), pointLightFbos[0]->m_Faces[4].camera.getLookAt(), pointLightFbos[0]->m_Faces[4].camera.getUp());
		if (Input::isKeyJustPressed(SDLK_6))
			camera.set(pointLightFbos[0]->m_Faces[5].camera.getPosition(), pointLightFbos[0]->m_Faces[5].camera.getLookAt(), pointLightFbos[0]->m_Faces[5].camera.getUp());

		
		glCullFace(GL_FRONT);
		renderer.setShadowShader(&shadowShader);
		for (size_t i = 0; i < lights.size(); i++)
		{
			ShadowMapPass(i);
		}
		renderer.setShadowShader(&pointLightShadowShader);
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			PointLightShadowMapPass(i);
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
	lightShader.setUniformMatrix4("spotLightVP[" + std::to_string(i) + "]", lightVP);
	lightShader.unbind();
}

void PointLightShadowMapPass(int i)
{
	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	glViewport(0, 0, CUBE_MAP_DIMENSIONS, CUBE_MAP_DIMENSIONS);
	root.setRenderPassIndex(i);
	//wallSwitch.setEnabled(false);
	
	pointLightShadowShader.bind();
	pointLightShadowShader.setUniform3("lightWorldPos", pointLights[i]->getPosition());
	pointLightFbos[i]->setLightPosition(pointLights[i]->getPosition());
	for (int j = 0; j < 6; j++)
	{
		pointLightFbos[i]->BindForWriting(pointLightFbos[i]->m_Faces[j].cubemapFace);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightVP = ShadowMapCube::s_Projection * pointLightFbos[i]->m_Faces[j].camera.getInverseViewMatrix();
		pointLightShadowShader.setUniformMatrix4("lightVP", lightVP);
		root.draw(glm::mat4(1.0));
	}
	pointLightShadowShader.unbind();
}

void RenderPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Window::getWidth(), Window::getHeight());
	root.setRenderPassIndex(-1);
	//wallSwitch.setEnabled(true);
	
	const glm::vec4& cc = Window::getClearColor();
	glClearColor(cc.x, cc.y, cc.z, cc.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < lights.size(); i++)
		shadowMapFBO.BindForReading(SHADOW_BASE_INDEX + i);

	for (size_t i = 0; i < pointLights.size(); i++)
		pointLightFbos[i]->BindForReading(SHADOW_CUBE_BASE_INDEX + i);

	lightShader.bind();
	const glm::mat4& VP = P * camera.getInverseViewMatrix();
	lightShader.setUniformMatrix4("VP", VP);

	//the lights can change. update them
	for (size_t i = 0; i < lights.size(); i++)
		lights[i]->setUniforms(lightShader, "spotLights[" + std::to_string(i) + "]");
	for (size_t i = 0; i < pointLights.size(); i++)
		pointLights[i]->setUniforms(lightShader, "pointLights[" + std::to_string(i) + "]");

	root.draw(glm::mat4(1.0));
	lightShader.unbind();
}