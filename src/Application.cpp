//
//  Application.cpp
//  

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "scene.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

#define PI 3.14159265358979323846


Application::Application(GLFWwindow* pWin) : window(pWin), camera(pWin)
{
	BaseModel* pModel;
	ConstantShader* pConstShader;
	PhongShader* pPhongShader;

	// create LineGrid model with constant color shader
	pModel = new LinePlaneModel(6.0f, 9.0f, 6, 9);
	pConstShader = new ConstantShader();
	pConstShader->color(Color(1, 1, 1));
	pModel->shader(pConstShader, true);
	models.push_back(pModel);

	this->createDuck();
}

void Application::update(float dTime)
{
	this->controlDuck(dTime);

	this->camera.update();
}

void Application::createDuck()
{

	PhongShader* shader = new PhongShader();
	this->duck = new Duck();
	this->duck->shader(shader, true);
	this->duck->loadModel(ASSET_DIRECTORY "duck.dae");

	this->models.push_back(this->duck);

	Matrix mat;
	mat.scale(5);
	this->duck->getModel()->transform(this->duck->getModel()->transform() * mat);

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