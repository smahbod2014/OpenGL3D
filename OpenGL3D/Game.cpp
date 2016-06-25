#include "OpenGL3D.h"

class Game : public OpenGL3D
{
private:
	Camera* camera;
	Renderer* renderer;
	SkyboxRenderer* skyboxRenderer;
	TerrainRenderer* terrainRenderer;
	DirectionalLight* dLight;
	Geode* tree;
	std::vector<glm::mat4> transformations;
	Terrain* terrain;
public:
	Game(const std::string& name, int width, int height) :
		OpenGL3D(name, width, height) {}

	virtual ~Game() {}

	virtual void init()
	{
		ModelCache::loadModel("tree", "Models/pine.obj");
		TextureManager::loadTexture("pine", "Textures/pine.png");
		TextureManager::loadTexture("wtf", "Textures/wtf face.png");
		TextureManager::loadTexture("grass", "Textures/grass.png");
		TextureManager::loadTexture("flowers", "Textures/grassFlowers.png");
		TextureManager::loadTexture("mud", "Textures/mud.png");
		TextureManager::loadTexture("path", "Textures/path.png");
		TextureManager::loadTexture("blendMap", "Textures/blendMap.png");
		TextureManager::loadCubeMap("clouds", "Skyboxes/Clouds");

		camera = new Camera();
		camera->translate(glm::vec3(0, 5, 0));
		camera->rotate(glm::vec3(0, 1, 0), -45);
		renderer = new Renderer();
		dLight = new DirectionalLight(glm::vec3(1, -0.5, 0), glm::vec3(1, 1, 1), 100);
		renderer->loadDirectionalLight(dLight);

		skyboxRenderer = new SkyboxRenderer("clouds");

		
		//cube->setTextureID(TextureManager::getTexture("pine"));

		terrainRenderer = new TerrainRenderer();
		terrainRenderer->loadDirectionalLight(dLight);
		terrain = new Terrain(0, -1, "grass", "mud", "flowers", "path", "blendMap", "Textures/s2.png");
		//terrain = new Terrain(0, -1, "grass", "mud", "flowers", "path", "blendMap");
		tree = new Geode("tree", renderer);
		tree->setTextureID(TextureManager::getTexture("pine"));

		Random* random = new Random();
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
		}
	}

	virtual void update(float dt)
	{
		camera->input(dt);
		catchError(1);
		//std::cout << "hi" << std::endl;
	}

	virtual void render()
	{
		
		terrainRenderer->render(terrain, camera);

		skyboxRenderer->render(camera);

		for (int i = 0; i < transformations.size(); i++) {
			tree->draw(transformations[i], camera);
		}
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