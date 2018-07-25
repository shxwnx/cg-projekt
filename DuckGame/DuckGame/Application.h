#pragma once

#include <stdio.h>
#include <list>
#include "Camera.h"
#include "PhongShader.h"
#include "ConstantShader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BaseModel.h"
#include "Duck.h"
#include "Spawner.h"


class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
	void update(float dTime);
    void draw();
    void end();
protected:
    Vector calc3DRay( float x, float y, Vector& Pos);
    Camera camera;
    ModelList models;
    GLFWwindow* window;

	//Duck
	Duck* duck;
	void createDuck();
	void controlDuck(float dTime);

	//Spawner
	Spawner* spawner;
	void createSpawner();

};
