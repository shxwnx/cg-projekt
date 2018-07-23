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


	///////////////////////////acceleration///////////////////////////
	/*if (this->leftRight == 0.0f) { //no key pressed
		this->speedLeftRight = 0.0f; 
	}
	else if (this->leftRight < 0.0f && -maxSpeedLeftRight < this->speedLeftRight) { //right
		this->speedLeftRight -= maxSpeedLeftRight / 10;
		//an -maxSpeedLeftRight * this->leftRight annähern
	}
	else if (this->leftRight > 0.0f && maxSpeedLeftRight > this->speedLeftRight) { //left
		this->speedLeftRight += maxSpeedLeftRight / 10;
		//an -maxSpeedLeftRight * this->leftRight annähern
	}*/
	//////////////////////////////////////////////////////////////////


	///////////////////////forward and backward///////////////////////
	if (duckTransform.translation().Z >= maxZ && this->forwardBackward < 0.0f) { //border (bottom)
		forwardBackwardMatrix.translation(0, 0, 0);
		this->speedForwardBackward = 0.0f;
	}
	else if (duckTransform.translation().Z <= -maxZ && this->forwardBackward > 0.0f) { //border (top)
		forwardBackwardMatrix.translation(0, 0, 0);
		this->speedForwardBackward = 0.0f;
	}
	else {
		//forwardBackwardMatrix.translation(0, 0, -maxSpeedForwardBackward * this->forwardBackward * dtime); //without acceleration

		this->speedForwardBackward = this->calculateSpeed(maxSpeedForwardBackward, this->speedForwardBackward, this->forwardBackward);
		forwardBackwardMatrix.translation(0, 0, -this->speedForwardBackward * dtime);
	}
	std::cout << this->speedForwardBackward << std::endl;
	//////////////////////////////////////////////////////////////////


	//////////////////////////right and left//////////////////////////
	if (duckTransform.translation().X >= maxX && this->leftRight < 0.0f) { //border (right)
		leftRightMatrix.translation(0, 0, 0);
		this->speedLeftRight = 0.0f;
	}
	else if (duckTransform.translation().X <= -maxX && this->leftRight > 0.0f) { //border (left)
		leftRightMatrix.translation(0, 0, 0);
		this->speedLeftRight = 0.0f;
	}
	else {
		//leftRightMatrix.translation(-maxSpeedLeftRight * this->leftRight * dtime, 0, 0);	//without acceleration

		this->speedLeftRight = this->calculateSpeed(maxSpeedLeftRight, this->speedLeftRight, this->leftRight);
		leftRightMatrix.translation(-this->speedLeftRight * dtime, 0, 0);					//with acceleration
		
		//this->speedLeftRight = -maxSpeedLeftRight * this->leftRight * dtime;
		//std::cout <<  this->speedLeftRight;
	}
	//std::cout << this->speedLeftRight << std::endl;
	/////////////////////////////////////////////////////////////////


	//////////////////////////////slope//////////////////////////////
	//Matrix mat3;
	//mat3.rotationY(this->speedLeftRight*100);
	//duckTransform.rotationY(this->speedLeftRight * 100);
	/////////////////////////////////////////////////////////////////


	this->model->transform(this->model->transform() * forwardBackwardMatrix * leftRightMatrix);

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