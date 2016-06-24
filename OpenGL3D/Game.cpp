#include "OpenGL3D.h"

class Game : public OpenGL3D
{
private:
	Camera* camera;
	Renderer* renderer;
	DirectionalLight* dLight;
	Geode* cube;
public:
	Game(const std::string& name, int width, int height) :
		OpenGL3D(name, width, height) {}

	virtual ~Game() {}

	virtual void init()
	{
		ModelCache::loadModel("cube", "Models/cube.obj");
		TextureManager::loadTexture("wtf", "Textures/wtf face.png");
		camera = new Camera();
		renderer = new Renderer();
		renderer->setCamera(camera);
		dLight = new DirectionalLight(glm::vec3(0, -1, 0), glm::vec3(1, 1, 1), 100);
		renderer->loadDirectionalLight(dLight);

		cube = new Geode("cube", renderer);
		cube->setTextureID(TextureManager::getTexture("wtf"));
	}

	virtual void update(float dt)
	{
		camera->input(dt);
		renderer->updateCamera();
		catchError(1);
		//std::cout << "hi" << std::endl;
	}

	virtual void render()
	{
		cube->draw();
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