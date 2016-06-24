#include "OpenGL3D.h"

class Game : public OpenGL3D
{
private:
	Camera* camera;
	Renderer* renderer;
	SkyboxRenderer* skyboxRenderer;
	TerrainRenderer* terrainRenderer;
	DirectionalLight* dLight;
	Geode* cube;
	Terrain* terrain;
public:
	Game(const std::string& name, int width, int height) :
		OpenGL3D(name, width, height) {}

	virtual ~Game() {}

	virtual void init()
	{
		ModelCache::loadModel("cube", "Models/cube.obj");
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
		dLight = new DirectionalLight(glm::vec3(-1, -0.5, 0), glm::vec3(1, 1, 1), 100);
		renderer->loadDirectionalLight(dLight);

		skyboxRenderer = new SkyboxRenderer("clouds");

		cube = new Geode("cube", renderer);
		cube->setTextureID(TextureManager::getTexture("wtf"));

		terrainRenderer = new TerrainRenderer();
		terrainRenderer->loadDirectionalLight(dLight);
		terrain = new Terrain(0, -1, "grass", "mud", "flowers", "path", "blendMap", "Textures/heightmap.png");
	}

	virtual void update(float dt)
	{
		camera->input(dt);
		catchError(1);
		//std::cout << "hi" << std::endl;
	}

	virtual void render()
	{
		cube->draw(camera);
		
		terrainRenderer->render(terrain, camera);

		skyboxRenderer->render(camera);
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