#include "OpenGL3D.h"
#include "WaterTile.h"
#include "WaterRenderer.h"
#include "WaterFramebuffers.h"
#include "GUIRenderer.h"
#include "Entity.h"
#include "ShadowMapMasterRenderer.h"
#include "Fbo.h"
#include "PostProcessing.h"

class Game : public OpenGL3D
{
private:
	Camera* camera;
	Renderer* renderer;
	SkyboxRenderer* skyboxRenderer;
	TerrainRenderer* terrainRenderer;
	WaterRenderer* waterRenderer;
	GUIRenderer* guiRenderer;
	ShadowMapMasterRenderer* shadowMapRenderer;
	DirectionalLight* dLight;
	Geode* tree, *plane;
	std::vector<glm::mat4> transformations;
	Terrain* terrain;
	WaterTile* water;
	WaterFramebuffers* waterfbos;
	GUITexture* reflectionGUI, *refractionGUI, *shadowMapGUI;
	std::vector<Entity*> entities;
	Fbo* multisampleFbo, *outputFbo;
	Entity* barrel, *crate, *boulder, *lantern;
public:
	Game(const std::string& name, int width, int height) :
		OpenGL3D(name, width, height) {}

	virtual ~Game() {}

	virtual void init()
	{
		ModelCache::loadModel("tree", "Models/pine.obj");
		ModelCache::loadModel("cherry", "Models/cherry.obj");
		//ModelCache::loadModel("squid_tree", "Models/squid_tree.obj");
		ModelCache::loadModel("barrel", "Models/barrel.obj");
		ModelCache::loadModel("boulder", "Models/boulder.obj");
		ModelCache::loadModel("crate", "Models/crate.obj");
		ModelCache::loadModel("lantern", "Models/lantern.obj");
		TextureManager::loadTexture("squid_tree", "Textures/squid_tree.png");
		TextureManager::loadTexture("pine", "Textures/pine.png");
		TextureManager::loadTexture("grass", "Textures/grass.png");
		TextureManager::loadTexture("flowers", "Textures/grassFlowers.png");
		TextureManager::loadTexture("mud", "Textures/mud.png");
		TextureManager::loadTexture("path", "Textures/path.png");
		TextureManager::loadTexture("blendMap", "Textures/blendMap.png");
		TextureManager::loadTexture("dudvMap", "Textures/waterDUDV2.png");
		TextureManager::loadTexture("normalMap", "Textures/matchingNormalMap.png");
		TextureManager::loadCubeMap("clouds", "Skyboxes/Clouds");
		TextureManager::loadTexture("barrel", "Textures/barrel.png");
		TextureManager::loadTexture("boulder", "Textures/boulder.png");
		TextureManager::loadTexture("crate", "Textures/crate.png");
		TextureManager::loadTexture("barrelNormal", "Textures/barrelNormal.png");
		TextureManager::loadTexture("boulderNormal", "Textures/boulderNormal.png");
		TextureManager::loadTexture("crateNormal", "Textures/crateNormal.png");
		TextureManager::loadTexture("barrelS", "Textures/barrelS.png");
		TextureManager::loadTexture("cherry", "Textures/cherry.png");
		TextureManager::loadTexture("cherryS", "Textures/cherryS.png");
		TextureManager::loadTexture("lantern", "Textures/lantern.png");
		TextureManager::loadTexture("lanternS", "Textures/lanternS.png");

		camera = new Camera();
		camera->translate(glm::vec3(5, 5, 10));
		//camera->rotate(glm::vec3(0, 1, 0), -90);
		renderer = new Renderer();
		dLight = new DirectionalLight(glm::vec3(-1, -0.5, 0), glm::vec3(1, 1, 1), 100);
		renderer->loadDirectionalLight(dLight);
		renderer->loadClipPlane(0, -1, 0, 15);

		skyboxRenderer = new SkyboxRenderer("clouds", 500.0f, 0.5f);

		
		//cube->setTextureID(TextureManager::getTexture("pine"));

		terrainRenderer = new TerrainRenderer();
		terrainRenderer->loadDirectionalLight(dLight);
		//terrain = new Terrain(-0.5f, -0.5f, 64.0f, "grass", "mud", "flowers", "path", "blendMap", "Textures/s3.png");
		terrain = new Terrain(-0.5f, -0.5f, 512, 70.0f, 128, "grass", "mud", "flowers", "path", "blendMap");
		water = new WaterTile(0, 0, -2, terrain->getSize() / 2, 0.06f, "dudvMap", "normalMap");
		tree = new Geode("tree", renderer);
		tree->setTextureID(TextureManager::getTexture("pine"));

		Random r(0);
		for (int i = 0; i < 1200; i++) {
			if (i % 10 == 0) {
				float x = terrain->getX() + r.nextFloat() * terrain->getSize();// -TERRAIN_SIZE / 2.0f;
				float z = terrain->getZ() + r.nextFloat() * terrain->getSize();// -TERRAIN_SIZE / 2.0f;
				float y = terrain->getHeightAtLocation(x, z) - 0.25f;
				if (y < water->height)
					continue;
				glm::mat4 m;
				m[3][0] = x;
				m[3][1] = y;
				m[3][2] = z;
				transformations.push_back(m);

				Entity* tree = new Entity("cherry", "cherry");
				tree->translate(x, y, z);
				tree->setSpecularProperties(10.0f, 1.0f);
				tree->setSpecularMap("cherryS");
				entities.push_back(tree);
			}
		}

		/*TextureData td("Textures/treemap.png");
		for (int z = 0; z < td.height; z++) {
			for (int x = 0; x < td.width; x++) {
				if (td.getRed(x, z) == 0xff) {
					float xx = terrain->getX() + (float)x / td.width * terrain->getSize();// -TERRAIN_SIZE / 2.0f;
					float zz = terrain->getZ() + (float)z / td.height * terrain->getSize();// -TERRAIN_SIZE / 2.0f;
					float yy = terrain->getHeightAtLocation(xx, zz) - 0.25f;
					glm::mat4 m;
					m[3][0] = xx;
					m[3][1] = yy;
					m[3][2] = zz;
					transformations.push_back(m);

					
					Entity* tree = new Entity("tree", "pine");
					tree->translate(xx, yy, zz);
					entities.push_back(tree);
				}
			}
		}*/

		waterfbos = new WaterFramebuffers();
		//water = new WaterTile(terrain->getX() + 30, terrain->getZ() + 30, 6, 25, 0.06f, "dudvMap", "normalMap");
		
		waterRenderer = new WaterRenderer(waterfbos);
		waterRenderer->loadDirectionalLight(dLight);

		//plane = new Geode("plane", renderer);
		//plane->setTextureID(waterfbos->reflectionTexture);

		shadowMapRenderer = new ShadowMapMasterRenderer(camera);
		terrainRenderer->setShadowMapTextureID(shadowMapRenderer->getShadowMapTexture());
		waterRenderer->setShadowMapTextureID(shadowMapRenderer->getShadowMapTexture());

		guiRenderer = new GUIRenderer();
		reflectionGUI = new GUITexture(waterfbos->reflectionTexture, glm::vec2(-0.85f, 0.85f), glm::vec2(0.15f, 0.15f));
		refractionGUI = new GUITexture(waterfbos->refractionTexture, glm::vec2(0.85f, 0.85f), glm::vec2(0.15f, 0.15f));
		//shadowMapGUI = new GUITexture(shadowMapRenderer->getShadowMapTexture(), glm::vec2(0.85f, 0.55f), glm::vec2(0.15f, 0.15f));
		shadowMapGUI = new GUITexture(shadowMapRenderer->getShadowMapTexture(), glm::vec2(-0.85f, -0.85f), glm::vec2(0.15f, 0.15f));

		multisampleFbo = new Fbo(Window::getWidth(), Window::getHeight(), 4);
		outputFbo = new Fbo(Window::getWidth(), Window::getHeight(), DepthBufferType::DEPTH_RENDER_BUFFER);

		//add the crate, barrel, and boulder for normal mapping
		barrel = new Entity("barrel", "barrel");
		boulder = new Entity("boulder", "boulder");
		crate = new Entity("crate", "crate");
		lantern = new Entity("lantern", "lantern");
		barrel->setNormalMap("barrelNormal");
		boulder->setNormalMap("boulderNormal");
		crate->setNormalMap("crateNormal");
		barrel->setSpecularMap("barrelS");
		lantern->setSpecularMap("lanternS");
		barrel->translate(40, 15, 0);
		boulder->translate(20, 15, 0);
		crate->translate(2, 15, 0);
		crate->scale(0.05f, 0.05f, 0.05f);
		lantern->translate(20, 0, 15);
		entities.push_back(barrel);
		entities.push_back(boulder);
		entities.push_back(crate);
		entities.push_back(lantern);
	}

	virtual void update(float dt)
	{
		camera->input(dt);
		water->update(dt);
		float rotSpeed = 45.0f * dt;
		barrel->rotate(0, 1, 0, rotSpeed);
		boulder->rotate(0, 1, 0, rotSpeed);
		crate->rotate(0, 1, 0, rotSpeed);
		catchError(1);
	}

	virtual void render()
	{
		shadowMapRenderer->render(entities, dLight);
		terrainRenderer->loadShadowSpaceMatrix(shadowMapRenderer->getToShadowMapSpaceMatrix());
		//waterRenderer->loadShadowSpaceMatrix(shadowMapRenderer->getToShadowMapSpaceMatrix());
		
		//Reflection pass-------------------
		waterfbos->bindReflectionFrameBuffer();
		renderer->loadClipPlane(0, 1, 0, -water->height + 0.07f);
		terrainRenderer->loadClipPlane(0, 1, 0, -water->height + 0.07f);
		float distance = 2.0f * (camera->getPosition().y - water->height);
		camera->translate(glm::vec3(0, -distance, 0));
		camera->invertPitch();
		terrainRenderer->render(terrain, camera);
		skyboxRenderer->render(camera);
		renderer->render(entities, camera);
		camera->translate(glm::vec3(0, distance, 0));
		camera->invertPitch();
		
		//Refraction pass-------------------
		waterfbos->bindRefractionFrameBuffer();
		renderer->loadClipPlane(0, -1, 0, water->height + 0.07f);
		terrainRenderer->loadClipPlane(0, -1, 0, water->height + 0.07f);
		terrainRenderer->render(terrain, camera);
		skyboxRenderer->render(camera);
		renderer->render(entities, camera);
		waterfbos->unbindCurrentFrameBuffer();

		//Normal pass-----------------------
		multisampleFbo->bindFramebuffer();
		terrainRenderer->render(terrain, camera);
		skyboxRenderer->render(camera);
		renderer->render(entities, camera);
		waterRenderer->render(water, camera);
		multisampleFbo->unbindFramebuffer();
		multisampleFbo->resolveToFbo(outputFbo);
		PostProcessing::doPostProcessing(outputFbo->getColorTexture());
		//guiRenderer->render(reflectionGUI);
		//guiRenderer->render(refractionGUI);
		//guiRenderer->render(shadowMapGUI);
	}

	virtual void finish()
	{

	}
};

int main(int argc, char* argv[])
{
	Game game("Water", 1440, 810);
	game.start();
	return 0;
}