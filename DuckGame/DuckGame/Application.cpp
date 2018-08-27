#include "Application.h"

#include "Model.h"

#include "PhongShader.h"
#include "ConstantShader.h"
#include "FontShader.h"

#define ASSET_DIRECTORY "../../assets/"
#define PI 3.14159265358979323846


Application::Application()
{
}

Application::Application(GLFWwindow* pWin) : window(pWin), camera(pWin)
{
	this->timePassed = 0.0f;
	BaseModel* pModel;
	ConstantShader* pConstShader;
	PhongShader* pPhongShader;

	// create LineGrid model with constant color shader
	/*pModel = new LinePlaneModel(6.0f, 9.0f, 6, 9);
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 1, 1));
	pModel->shader(pConstShader, true);
	models.push_back(pModel);*/



	this->createSpawner();
	this->createDuck();
	this->createWater(6.0f, 9.0f, 6, 9);
	this->createSkyBox();
	this->createUI();

}

void Application::update(float dTime)
{
	this->timePassed += dTime;
	this->controlDuck(dTime);
	this->spawner->update(dTime);
	this->camera.update();
	this->uiService->update(dTime);
}

void Application::createDuck()
{
	PhongShader* shader = new PhongShader();
	this->duck = new Duck(this->spawner->getOutputModels());
	this->duck->shader(shader, true);
	this->duck->loadModel(ASSET_DIRECTORY "newduck.dae");

	this->models.push_back(this->duck);
}

void Application::controlDuck(float dTime)
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
	this->duck->update(dTime);
}

void Application::createSpawner()
{
	auto shader = new PhongShader();
	// countRows, countObjets, spacing, spawnTime, speed, acceleration, accelerateTime
	this->spawner = new Spawner(5, 3, 1.0f, 0.4f, 3.0f, 0.1f, 2.0f);
	this->spawner->shader(shader, true);

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
	this->water = new Water(sizeX, sizeZ, segmentsX, segmentsZ);

	//PhongShader* shader = new PhongShader();
	ConstantShader* shader = new ConstantShader();
	shader->color(Color(1, 1, 1));
	this->water->shader(shader, true);
	this->water->loadModel();

	//Eigenen Shader einfügen
	models.push_back(this->water);
}

void Application::createSkyBox()
{
	auto model = new Model(ASSET_DIRECTORY "skybox.obj", false);
	model->shader(new PhongShader(), true);
	this->models.push_back(model);
}

void Application::createUI()
{
	this->uiService = new UIService(ASSET_DIRECTORY "roboto.ttf" );
	this->uiService->shader(new FontShader(Vector(0,0,0)), true);
	this->models.push_back(this->uiService);
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
	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 2. setup shaders and draw models
	for (ModelList::iterator it = models.begin(); it != models.end(); ++it)
	{
		(*it)->draw(camera);
	}

	// 3. check once per frame for opengl errors
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
