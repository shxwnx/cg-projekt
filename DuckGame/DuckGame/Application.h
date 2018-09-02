#pragma once

#include <stdio.h>
#include <list>

#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "BaseModel.h"
#include "vector.h"
#include "Camera.h"

#include "Duck.h"
#include "Spawner.h"
#include "Water.h"
#include "Pool.h"
#include "UIService.h"

class Application
{
public:
	Application();
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
	void update(float dTime);
    void draw();
    void end();

	float getTimePassed();
protected:
    Vector calc3DRay( float x, float y, Vector& Pos);
    Camera camera;
    ModelList models;
    GLFWwindow* window;
	bool isStopped;
	GLuint depthTexture;

	// Duck
	Duck* duck;
	void createDuck();
	void controlDuck();

	// Spawner
	Spawner* spawner;
	void createSpawner();

	// Water
	Water* water;
	void createWater(float sizeX, float sizeZ, int segmentsX, int segmentsZ);

	// Pool
	Pool* pool;
	void createPool();

	// Skybox 
	void createSkyBox();

	// UI
	UIService* uiService;
	void createUI();

	//	General
	float timePassed;
	void reset();

};
