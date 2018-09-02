//
//  Pool.cpp
//  


#include "Pool.h"
#include <iostream>


Pool::Pool(Spawner* spawner): 
	scale (1.0f),
	spawner (spawner)
{
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
	auto toonShader = dynamic_cast<ToonShader *>(this->shader());
	toonShader->addTime(dtime);
	toonShader->setSpeed(this->spawner->getSpeed());
}

void Pool::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}

void Pool::transform(Matrix t) {
	this->model->transform(t);
}