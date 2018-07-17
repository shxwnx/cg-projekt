//
//  Duck.cpp
//  


#include "Duck.h"
#include <iostream>

#define PI 3.14159265358979323846

Duck::Duck()
{
	this->rotation = 0.0;
	this->forwardBackward = 0.0;
	this->leftRight = 0.0;
}

Duck::~Duck()
{
	this->duckModel= NULL;
}

bool Duck::loadModel(const char* DuckFile)
{
	this->duckModel = new Model(DuckFile, false);

	if (!duckModel->load(DuckFile, false)) {
		return false;
	}

	this->duckModel->shader(this->pShader, false);

	return true;
}

void Duck::steer(float ForwardBackward, float LeftRight)
{
	this->forwardBackward = ForwardBackward;
	this->leftRight = LeftRight;
}

void Duck::update(float dtime)
{
	Matrix mat1;	//forwardBackward 
	Matrix mat2;	//leftRight

	Matrix duckTransform;
	float maxX = 50;
	float maxZ = 50;

	duckTransform = this->duckModel->transform();

	mat1.translation(0 , 0, -1*this->forwardBackward * dtime);
	mat2.translation(-2*this->leftRight * dtime, 0, 0);
	//mat2.rotationY(PI / 4 * this->leftRight * dtime);


	this->duckModel->transform(this->duckModel->transform() * mat1 * mat2);

}

void Duck::draw(const BaseCamera& Cam)
{
	this->duckModel->draw(Cam);
}
