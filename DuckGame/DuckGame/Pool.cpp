//
//  Pool.cpp
//  


#include "Pool.h"
#include <iostream>


Pool::Pool(Camera * cam)
{
	this->camera = cam;
	this->scale = 4.0f;
}

Pool::~Pool()
{
	this->model = NULL;
}

bool Pool::loadModel(const char* file)
{
	this->model = new Model(file, false, this->scale);
	this->model->shader(this->pShader, true);

	return true;
}


void Pool::update(float dtime)
{

}

void Pool::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}
