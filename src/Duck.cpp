//
//  Duck.cpp
//  


#include "Duck.h"
#include <iostream>

#define PI 3.14159265358979323846

Duck::Duck()
{
	this->slope = 0.0;
	this->forwardBackward = 0.0;
	this->leftRight = 0.0;
	this->speedLeftRight = 0.0;
}

Duck::~Duck()
{
	this->model = NULL;
}

bool Duck::loadModel(const char* file)
{
	this->model = new Model(file, false);

	if (!this->model->load(file, false)) {
		return false;
	}
	this->model->shader(this->pShader, false);

	Matrix mRotate;
	Matrix mPosition;
	
	mRotate.translation(0, 0, 0);
	mRotate.rotationY(-PI / 2);
	mPosition.translation(0, 0, -2.0f);

	this->model->transform(mPosition);

	return true;
}

void Duck::steer(float ForwardBackward, float LeftRight)
{
	this->forwardBackward = ForwardBackward;
	this->leftRight = LeftRight;
}

void Duck::update(float dtime)
{
	Matrix forwardBackwardMatrix;	//Translation
	Matrix leftRightMatrix;			//Translation

	Matrix duckTransform = this->model->transform();
	float maxX = 2.5f;						//leftRight border
	float maxZ = 2.5f;						//forwardBackward border
	float maxSpeedLeftRight = 1.5f;			//max leftRight speed
	float maxSpeedForwardBackward = 0.5f;	//max forwardBackward speed


	///////////////////////forward and backward///////////////////////
	if (duckTransform.translation().Z >= maxZ && this->forwardBackward < 0.0f) { //border (bottom)
		this->speedForwardBackward = 0.0f;
	}
	else if (duckTransform.translation().Z <= -maxZ && this->forwardBackward > 0.0f) { //border (top)
		this->speedForwardBackward = 0.0f;
	}
	else {
		this->speedForwardBackward = this->calculateSpeed(maxSpeedForwardBackward, this->speedForwardBackward, this->forwardBackward);	
	}
	forwardBackwardMatrix.translation(0, 0, -this->speedForwardBackward * dtime);
	//std::cout << this->speedForwardBackward << std::endl;
	//////////////////////////////////////////////////////////////////


	//////////////////////////right and left//////////////////////////
	if (duckTransform.translation().X >= maxX && this->leftRight < 0.0f) { //border (right)
		this->speedLeftRight = 0.0f;
	}
	else if (duckTransform.translation().X <= -maxX && this->leftRight > 0.0f) { //border (left)
		this->speedLeftRight = 0.0f;
	}
	else {
		//leftRightMatrix.translation(-maxSpeedLeftRight * this->leftRight * dtime, 0, 0);	//without acceleration
		this->speedLeftRight = this->calculateSpeed(maxSpeedLeftRight, this->speedLeftRight, this->leftRight);
	}
	leftRightMatrix.translation(-this->speedLeftRight * dtime, 0, 0);
	//std::cout << this->speedLeftRight << std::endl;
	/////////////////////////////////////////////////////////////////


	//////////////////////////////slope//////////////////////////////
	Matrix newSlope;
	Matrix oldSlope;

	newSlope.rotationY(this->speedLeftRight/5);
	oldSlope.rotationY(-this->slope);
	/////////////////////////////////////////////////////////////////


	this->model->transform(this->model->transform() * oldSlope * forwardBackwardMatrix * leftRightMatrix * newSlope);
	this->slope = this->speedLeftRight/5;
}

void Duck::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}

Model* Duck::getModel() {
	return this->model;
}

float Duck::calculateSpeed(float maxSpeed, float currentSpeed, float directionValue) {
	float speed = 0.0f;
	if (directionValue == 0.0f) { //no key pressed
		speed = 0.0f;
	}
	else if (directionValue < 0.0f && -maxSpeed < currentSpeed) { //right
		speed = currentSpeed - maxSpeed / 10;
	}
	else if (directionValue > 0.0f && maxSpeed > currentSpeed) { //left
		speed = currentSpeed + maxSpeed / 10;
	} 
	else {
		speed = currentSpeed;
	}

	return speed;
}