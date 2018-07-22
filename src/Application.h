//
//  Application.hpp
//  

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "tank.h"
#include "Duck.h"


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

};

#endif /* Application_hpp */
