#include "OpenGL3D.h"
#include "WaterTile.h"
#include "WaterRenderer.h"
#include "WaterFramebuffers.h"
#include "GUIRenderer.h"

class Game : public OpenGL3D
{
private:
	Camera* camera;
	Renderer* renderer;
	SkyboxRenderer* skyboxRenderer;
	TerrainRenderer* terrainRenderer;
	WaterRenderer* waterRenderer;
	GUIRenderer* guiRenderer;
	DirectionalLight* dLight;
	Geode* tree, *plane;
	std::vector<glm::mat4> transformations;
	Terrain* terrain;
	WaterTile* water;
	WaterFramebuffers* waterfbos;
	GUITexture* reflectionGUI, *refractionGUI, *refractionDepthGUI;
public:
	Game(const std::string& name, int width, int height) :
		OpenGL3D(name, width, height) {}

	virtual ~Game() {}

	virtual void init()
	{
		ModelCache::loadModel("tree", "Models/pine.obj");
		ModelCache::loadPlane("plane", 25);
		TextureManager::loadTexture("pine", "Textures/pine.png");
		TextureManager::loadTexture("wtf", "Textures/wtf face.png");
		TextureManager::loadTexture("grass", "Textures/grass.png");
		TextureManager::loadTexture("flowers", "Textures/grassFlowers.png");
		TextureManager::loadTexture("mud", "Textures/mud.png");
		TextureManager::loadTexture("path", "Textures/path.png");
		TextureManager::loadTexture("blendMap", "Textures/blendMap.png");
		TextureManager::loadTexture("dudvMap", "Textures/waterDUDV2.png");
		TextureManager::loadTexture("normalMap", "Textures/matchingNormalMap.png");
		TextureManager::loadCubeMap("clouds", "Skyboxes/Clouds");

		camera = new Camera();
		camera->translate(glm::vec3(0, 5, 0));
		camera->rotate(glm::vec3(0, 1, 0), -45);
		renderer = new Renderer();
		dLight = new DirectionalLight(glm::vec3(-1, -0.5, 0), glm::vec3(1, 1, 1), 100);
		renderer->loadDirectionalLight(dLight);
		renderer->loadClipPlane(0, -1, 0, 15);

		skyboxRenderer = new SkyboxRenderer("clouds");

		
		//cube->setTextureID(TextureManager::getTexture("pine"));

		terrainRenderer = new TerrainRenderer();
		terrainRenderer->loadDirectionalLight(dLight);
		terrain = new Terrain(0, -1, "grass", "mud", "flowers", "path", "blendMap", "Textures/s3.png");
		//terrain = new Terrain(0, -1, "grass", "mud", "flowers", "path", "blendMap");
		tree = new Geode("tree", renderer);
		tree->setTextureID(TextureManager::getTexture("pine"));

		/*Random* random = new Random();
		for (int i = 0; i < 600; i++) {
			if (i % 20 == 0) {
				float x = terrain->getX() + random->nextFloat() * TERRAIN_SIZE;// -TERRAIN_SIZE / 2.0f;
				float z = terrain->getZ() + random->nextFloat() * TERRAIN_SIZE;// -TERRAIN_SIZE / 2.0f;
				float y = terrain->getHeightAtLocation(x, z) - 0.25f;
				glm::mat4 m;
				m[3][0] = x;
				m[3][1] = y;
				m[3][2] = z;
				transformations.push_back(m);
			}
		}*/

		TextureData td("Textures/treemap.png");
		for (int z = 0; z < td.height; z++) {
			for (int x = 0; x < td.width; x++) {
				if (td.getRed(x, z) == 0xff) {
					float xx = terrain->getX() + (float)x / td.width * TERRAIN_SIZE;// -TERRAIN_SIZE / 2.0f;
					float zz = terrain->getZ() + (float)z / td.height * TERRAIN_SIZE;// -TERRAIN_SIZE / 2.0f;
					float yy = terrain->getHeightAtLocation(xx, zz) - 0.25f;
					glm::mat4 m;
					m[3][0] = xx;
					m[3][1] = yy;
					m[3][2] = zz;
					transformations.push_back(m);
				}
			}
		}

		waterfbos = new WaterFramebuffers();
		water = new WaterTile(30, -30, 6, 25, 0.06f, "dudvMap", "normalMap");
		waterRenderer = new WaterRenderer(waterfbos);
		waterRenderer->loadDirectionalLight(dLight);

		//plane = new Geode("plane", renderer);
		//plane->setTextureID(waterfbos->reflectionTexture);

		guiRenderer = new GUIRenderer();
		reflectionGUI = new GUITexture(waterfbos->reflectionTexture, glm::vec2(-0.75f, 0.75f), glm::vec2(0.25f, 0.25f));
		refractionGUI = new GUITexture(waterfbos->refractionTexture, glm::vec2(0.75f, 0.75f), glm::vec2(0.25f, 0.25f));
		refractionDepthGUI = new GUITexture(waterfbos->refractionDepthTexture, glm::vec2(0.75f, 0.25f), glm::vec2(0.25f, 0.25f));
	}

	virtual void update(float dt)
	{
		camera->input(dt);
		water->update(dt);
		catchError(1);
		//std::cout << "hi" << std::endl;
	}

	virtual void render()
	{
		//Reflection pass-------------------
		waterfbos->bindReflectionFrameBuffer();
		renderer->loadClipPlane(0, 1, 0, -water->height);
		terrainRenderer->loadClipPlane(0, 1, 0, -water->height);
		float distance = 2.0f * (camera->getPosition().y - water->height);
		//camera->setPositionNoLook(camera->getPosition() - glm::vec3(0, distance, 0));
		camera->translate(glm::vec3(0, -distance, 0));
		camera->invertPitch();
		terrainRenderer->render(terrain, camera);
		skyboxRenderer->render(camera);
		for (int i = 0; i < transformations.size(); i++) {
			tree->draw(transformations[i], camera);
		}
		//camera->setPositionNoLook(camera->getPosition() + glm::vec3(0, distance, 0));
		camera->translate(glm::vec3(0, distance, 0));
		camera->invertPitch();
		
		//Refraction pass-------------------
		waterfbos->bindRefractionFrameBuffer();
		renderer->loadClipPlane(0, -1, 0, water->height);
		terrainRenderer->loadClipPlane(0, -1, 0, water->height);
		terrainRenderer->render(terrain, camera);
		skyboxRenderer->render(camera);
		for (int i = 0; i < transformations.size(); i++) {
			tree->draw(transformations[i], camera);
		}

		//Normal pass-----------------------
		waterfbos->unbindCurrentFrameBuffer();
		terrainRenderer->render(terrain, camera);
		skyboxRenderer->render(camera);
		for (int i = 0; i < transformations.size(); i++) {
			tree->draw(transformations[i], camera);
		}
		waterRenderer->render(water, camera);
		//plane->draw(createTransformation(25, 10, -25, 0, 1, 0, 0, 1, 1, 1), camera);
		guiRenderer->render(reflectionGUI);
		guiRenderer->render(refractionGUI);
		//guiRenderer->render(refractionDepthGUI);
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