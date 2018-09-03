#include "Application.h"

#include "Model.h"

#include "PhongShader.h"
#include "ConstantShader.h"
#include "FontShader.h"
#include "WaterShader.h"
#include "BaseShader.h"
#include "ToonShader.h"

#define ASSET_DIRECTORY "../../assets/"
#define PI 3.14159265358979323846


Application::Application()
{
}

Application::Application(GLFWwindow* pWin) : 
	window(pWin), 
	camera(pWin),
	resetAvailable(false),
	timePassed(0.0f),
	isStopped(true)
{
	this->createSpawner();
	this->createDuck();

	this->createWater(12.0f, 36.0f, 60, 180);
	this->createPool();
	this->createSkyBox();
	this->createUI();
}

void Application::update(float dTime)
{
	if (!this->isStopped) {
		this->timePassed += dTime;
		this->controlDuck();
		this->duck->update(dTime);
		this->spawner->update(dTime);
		this->uiService->update(dTime);
		this->water->update(dTime);
		this->pool->update(dTime);

		if (this->duck->collisionDetected()) {
			this->reset();
			std::cout << "Application::update(): Collision detected!" << std::endl;
		}

		if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			this->resetAvailable = true;
		}
		if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS && this->resetAvailable) {
			this->resetAvailable = false;
			this->reset();
		}
	}
	else {
		if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			this->spawner->start();
			this->isStopped = false;
		}
	}

	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		exit(0);
	}
	

	this->camera.update();

}


void Application::createDuck()
{
	auto shader = new ToonShader();
	this->duck = new Duck(this->spawner->getOutputModels(), &this->camera);
	this->duck->shader(shader, true);
	this->duck->loadModel(ASSET_DIRECTORY "duck.dae");

	this->models.push_back(this->duck);
}

void Application::controlDuck()
{
	float fb = 0.0;
	float lr = 0.0;

	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			lr = 0.0f;
		}
		else lr = 1.0f;
	}
	else if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		lr = -1.0f;
	}

	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			fb = 0.0f;
		}
		else fb = 1.0f;
	}
	else if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		fb = -1.0f;
	}

	this->duck->steer(fb, lr);
}

void Application::createSpawner()
{
	// countRows, countObjets, spacing, spawnTime, speed, acceleration, accelerateTime
	this->spawner = new Spawner(9, 80, 1.25f, 0.2f, 5.0f, 0.1f, 2.0f);
	this->spawner->setCamera(&this->camera);

	std::vector<const char*> files = {
		ASSET_DIRECTORY "donut_brown.dae" ,
		ASSET_DIRECTORY "donut_pink.dae" ,
		ASSET_DIRECTORY "waterball_blue.dae" ,
		ASSET_DIRECTORY "waterball_colorful.dae",
		ASSET_DIRECTORY "waterball_rainbow.dae"
	};
	this->spawner->loadModels(files);
	this->models.push_back(this->spawner);
}

void Application::createWater(float sizeX, float sizeZ, int segmentsX, int segmentsZ) 
{
	this->water = new Water(sizeX, sizeZ, segmentsX, segmentsZ, this->spawner);

	WaterShader* shader = new WaterShader(Vector2D(sizeX, sizeZ));
	this->water->shader(shader, true);
	this->water->loadModel();
	if (!this->water->generateWaves()) {
		std::cout << "Application::createWater(): No WaterShader found!" << std::endl;
	}

	models.push_back(this->water);
}

void Application::createPool() 
{
	auto shader = new ToonShader();
	shader->setType(POOL);
	this->pool = new Pool(this->spawner);
	this->pool->shader(shader, true);
	this->pool->loadModel(ASSET_DIRECTORY "pool.dae");

	Matrix mPosition;
	mPosition.translation(0.0, -2.0, 0.0);
	this->pool->transform(mPosition);

	this->models.push_back(this->pool);
}

void Application::createSkyBox()
{
	auto model = new Model(ASSET_DIRECTORY "skybox.obj", false);
	model->shader(new PhongShader(), true);
	this->models.push_back(model);
}

void Application::createUI()
{
	this->uiService = new UIService(ASSET_DIRECTORY "roboto.ttf", &this->camera);
	this->models.push_back(this->uiService);
}

void Application::reset()
{
	this->isStopped = true;
	this->timePassed = 0.0f;
	this->duck->reset();
	this->spawner->stop();
	this->spawner->reset();
}

void Application::start()
{
	glEnable(GL_DEPTH_TEST);// enable depth-testing
	glDepthFunc(GL_LESS);	// depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Vector Application::calc3DRay(float x, float y, Vector& Pos)
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(this->window, &width, &height);

	// pixel coordinates of the mouse pointer into normalized image coordinates
	x = ((x * 2) / width) - 1;
	y = (((y * 2) / height) - 1) * -1;
	Vector temp = Vector(x, y, 0);

	// conversion into camera coordinates
	Matrix projectionMatrix = this->camera.getProjectionMatrix();
	projectionMatrix.invert();
	temp = projectionMatrix * temp;

	// conversion into world coordinates
	Matrix viewMatrix = this->camera.getViewMatrix();
	viewMatrix.invert();
	Vector temp2 = Vector(viewMatrix.transformVec3x3(temp)); // adjustment of direction (not origin)

	// plane intersection
	float s = 0;
	Pos.triangleIntersection(temp2, Vector(1, 0, 1), Vector(-1, 0, -1), Vector(-1, 0, 1), s);
	temp2 = Pos + temp2 * s;

	return temp2;
}
void Application::draw()
{
	// clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// setup shaders and draw models
	for (ModelList::iterator it = models.begin(); it != models.end(); ++it)
	{
		(*it)->draw(camera);
	}

	// check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}

void Application::end()
{
	for (ModelList::iterator it = models.begin(); it != models.end(); ++it)
		delete *it;

	models.clear();
}

float Application::getTimePassed()
{
	return this->timePassed;
}
