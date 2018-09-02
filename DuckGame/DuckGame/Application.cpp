#include "Application.h"

#include "Model.h"

#include "PhongShader.h"
#include "ConstantShader.h"
#include "FontShader.h"
#include "WaterShader.h"
#include "BaseShader.h"
#include "ToonShader.h"
#include "ShaderLightMapper.h"

#define ASSET_DIRECTORY "../../assets/"
#define PI 3.14159265358979323846


Application::Application()
{
}

Application::Application(GLFWwindow* pWin) : window(pWin), camera(pWin)
{
	this->timePassed = 0.0f;
	this->isStopped = true;

	this->createLight();
	this->createSpawner();
	this->createDuck();

	//this->createWater(6.0f, 9.0f, 60, 90);
	this->createWater(12.0f, 25.0f, 60, 125);
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
	}
	else {
		if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			this->spawner->start();
			this->isStopped = false;
		}
	}
	this->camera.update();

}

void Application::createLight() {

	auto dl = new DirectionalLight();
	dl->direction(Vector(0.2f, -1, 1));
	dl->color(Color(1.0f, 1.0f, 1.0f));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);

	Color c = Color(1.0f, 1.0f, 1.0f);
	Vector a = Vector(1.0f, 0, 0.1f);

	this->sun = new TriangleSphereModel(20.0f);
	this->sun->transform();
	this->sun->shader(new ToonShader());
	this->models.push_back(this->sun);
}

void Application::createDuck()
{
	auto shader = new ToonShader();
	this->duck = new Duck(this->spawner->getOutputModels(), &this->camera);
	this->duck->shader(shader, true);
	this->duck->loadModel(ASSET_DIRECTORY "newduck.dae");

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
	this->spawner = new Spawner(9, 3, 1.25f, 0.3f, 4.0f, 0.1f, 2.0f);
	this->spawner->setCamera(&this->camera);

	std::vector<const char*> files = {
		//ASSET_DIRECTORY "donut_brown.dae" ,
		ASSET_DIRECTORY "donut_pink.dae" ,
		ASSET_DIRECTORY "waterball_blue.dae" ,
		ASSET_DIRECTORY "waterball_colorful.dae"
	};
	this->spawner->loadModels(files);
	this->models.push_back(this->spawner);
}

void Application::createWater(float sizeX, float sizeZ, int segmentsX, int segmentsZ) {
	this->water = new Water(sizeX, sizeZ, segmentsX, segmentsZ, this->spawner);

	WaterShader* shader = new WaterShader(Vector2D(sizeX, sizeZ));
	shader->setDepthTexture(&this->depthTexture);
	this->water->shader(shader, true);
	this->water->loadModel();
	if (!this->water->generateWaves()) {
		std::cout << "Application::createWater(): No WaterShader found!" << std::endl;
	}
	models.push_back(this->water);
}

void Application::createPool() {
	auto shader = new ToonShader();
	shader->setType(POOL);
	this->pool = new Pool(this->spawner);
	this->pool->shader(shader, true);
	this->pool->loadModel(ASSET_DIRECTORY "pool.dae");

	Matrix mScale;
	Matrix mPosition;
	mScale.scale(96.5);
	mPosition.translation(0, -1.5, 0);
	this->pool->transform(mPosition*mScale);

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
	this->uiService = new UIService(ASSET_DIRECTORY "arial.ttf", &this->camera);
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
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
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

	//Umrechnung der Pixelkoordinaten des Mauszeigers in normalisierte Bildkoordinaten
	x = ((x * 2) / width) - 1;
	y = (((y * 2) / height) - 1) * -1;
	Vector temp = Vector(x, y, 0);

	//Umrechnung in Kamera-Koordinaten
	Matrix projectionMatrix = this->camera.getProjectionMatrix();
	projectionMatrix.invert();
	temp = projectionMatrix * temp;

	//Umrechnung in Weltkoordinaten
	Matrix viewMatrix = this->camera.getViewMatrix();
	viewMatrix.invert();
	Vector temp2 = Vector(viewMatrix.transformVec3x3(temp)); //Ánpassung der Richtung (nicht Ursprung)

	//Schnittpunkt mit der Ebene
	float s = 0;
	Pos.triangleIntersection(temp2, Vector(1, 0, 1), Vector(-1, 0, -1), Vector(-1, 0, 1), s);
	temp2 = Pos + temp2 * s;

	return temp2;
}
void Application::draw()
{
	//Generate texture
	//glGenTextures(1, &depthTexture);
	//glBindTexture(GL_TEXTURE_2D, depthTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512,
	//	512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models
	for (ModelList::iterator it = models.begin(); it != models.end(); ++it)
	{
		(*it)->draw(camera);
	}
	ShaderLightMapper::instance().deactivate();

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);

	////copy depth buffer to texture
	//glBindTexture(GL_TEXTURE_2D, depthTexture);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 512, 512);

	////enable texturing and bind the depth texture
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, depthTexture);

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
