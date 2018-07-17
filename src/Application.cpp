//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
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


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin)
{
    BaseModel* pModel;
    ConstantShader* pConstShader;
    PhongShader* pPhongShader;
    
    // create LineGrid model with constant color shader
    pModel = new LinePlaneModel(10, 10, 10, 10);
    pConstShader = new ConstantShader();
	pConstShader->color(Color(1,1,1));
    pModel->shader(pConstShader, true);
    Models.push_back( pModel );

    
    // Exercise 2
    /*
	pPhongShader = new PhongShader();
    pTank = new Tank();
    pTank->shader(pPhongShader, true);
    pTank->loadModels(ASSET_DIRECTORY "tank_bottom.dae", ASSET_DIRECTORY "tank_top.dae");
    Models.push_back( pTank );
	*/
    
    
    // Exercise 3
    /*
    Scene* pScene = new Scene();
    pScene->shader(new PhongShader(), true);
    pScene->addSceneFile(ASSET_DIRECTORY "scene.osh");
    Models.push_back(pScene);
    */
    
	///////////////////////////////////////////////////////////////////////////////////Laden der Duck///////////////////////////////////////////////////////////////////////////////////
	pPhongShader = new PhongShader();
	duck = new Duck();
	duck->shader(pPhongShader, true);
	duck->loadModel(ASSET_DIRECTORY "tank_bottom.dae");
	//duck->loadModel(ASSET_DIRECTORY "duck_highpoly.dae");
	Models.push_back(duck);

}

void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Application::update(float dTime)
{
    ///////////////////////////////////////////////////////////////////////////////////Steuerung der Duck///////////////////////////////////////////////////////////////////////////////////
	float fb = 0.0;
	float lr = 0.0;

	if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
		if (glfwGetKey(this->pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
			lr = 0.0f;
		}else lr = 1.0f;
	}else if (glfwGetKey(this->pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
		lr = -1.0f;
	}

	if (glfwGetKey(this->pWindow, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
		if (glfwGetKey(this->pWindow, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
			fb = 0.0f;
		}else fb = 1.0f;
	}else if (glfwGetKey(this->pWindow, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
		fb = -1.0f;
	}

	this->duck->steer(fb, lr);
	this->duck->update(dTime);

    Cam.update();
	
}

Vector Application::calc3DRay( float x, float y, Vector& Pos)
{
	int width = 0;
	int height= 0;
	glfwGetWindowSize(this->pWindow, &width, &height);

	//Umrechnung der Pixelkoordinaten des Mauszeigers in normalisierte Bildkoordinaten
	x = ((x * 2) / width) - 1;
	y = (((y * 2) / height) - 1) * -1;
	Vector temp = Vector(x, y, 0);

	//Umrechnung in Kamera-Koordinaten
	Matrix projectionMatrix = this->Cam.getProjectionMatrix();
	projectionMatrix.invert();
	temp = projectionMatrix * temp;

	//Umrechnung in Weltkoordinaten
	Matrix viewMatrix = this->Cam.getViewMatrix();
	viewMatrix.invert();
	Vector temp2 = Vector(viewMatrix.transformVec3x3(temp)); //Ánpassung der Richtung (nicht Ursprung)

	//Schnittpunkt mit der Ebene
	float s = 0;
	Pos.triangleIntersection(temp2, Vector(1, 0, 1), Vector(-1, 0, -1), Vector(-1, 0, 1), s);
	temp2 = Pos + temp2 *s;

    return temp2;
}

void Application::draw()
{
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
    
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}