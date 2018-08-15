//
//  Duck.cpp
//  


#include "Duck.h"
#include <iostream>

#define PI 3.14159265358979323846
#define FORWARDBACKWARD true
#define LEFTRIGHT false


Duck::Duck(std::vector<Model*> *obstacleModels)
{
	this->slope = 0.0;
	this->forwardBackward = 0.0;
	this->leftRight = 0.0;
	this->speedLeftRight = 0.0;
	this->obstacleModels = obstacleModels;
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

	Matrix position;
	Matrix scale;

	position.translation(0, 0, 2);
	scale.scale(5);

	this->model->transform(position * scale);

	return true;
}

void Duck::steer(float ForwardBackward, float LeftRight)
{
	this->forwardBackward = ForwardBackward;
	this->leftRight = LeftRight;
}

void Duck::update(float dtime)
{
	Matrix duckTransform = this->model->transform();

	//movement
	Matrix forwardBackwardMatrix;	//translation
	Matrix leftRightMatrix;			//translation
	float maxX = 2.5f;						//leftRight border
	float maxZ = 2.0f;						//forwardBackward border
	float maxSpeedLeftRight = 1.5f;			//max leftRight speed
	float maxSpeedForwardBackward = 0.5f;	//max forwardBackward speed


	this->speedForwardBackward = this->calculateSpeed(maxSpeedForwardBackward, this->speedForwardBackward, this->forwardBackward, duckTransform.translation().Z, maxZ, FORWARDBACKWARD);
	forwardBackwardMatrix.translation(0, 0, -this->speedForwardBackward * dtime);

	this->speedLeftRight = this->calculateSpeed(maxSpeedLeftRight, this->speedLeftRight, this->leftRight, duckTransform.translation().X, maxX, LEFTRIGHT);
	leftRightMatrix.translation(-this->speedLeftRight * dtime, 0, 0);


	//slope
	Matrix newSlope;
	Matrix oldSlope;
	float newSlopeValue = calculateSlope();

	newSlope.rotationY(newSlopeValue);
	oldSlope.rotationY(-this->slope);


	//transform
	this->model->transform(this->model->transform() * oldSlope * forwardBackwardMatrix * leftRightMatrix * newSlope);
	this->slope = newSlopeValue;

	//check collision
	if (this->checkCollision()) std::cout << "GAME OVER: Collision detected!" << std::endl;
}

void Duck::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}

float Duck::calculateSpeed(float maxSpeed, float currentSpeed, float directionValue, float translation, float border, bool direction) {
	float speed = 0.0f;

	//falls die Duck weiter nach vorne als nach nach hinten kann
	/*if (direction == FORWARDBACKWARD) {
		if ((translation < (border + 1.0f) && directionValue < 0.0f) || (translation > -border && directionValue > 0.0f)) {
			if (directionValue == 0.0f) { //no key pressed !!!!!!!
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
		}

	} else {*/
		if ((translation < border && directionValue < 0.0f) || (translation > -border && directionValue > 0.0f)) {
			if (directionValue == 0.0f) { //no key pressed !!!!!!!
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
		}
	//}
	return speed;
}

float Duck::calculateSlope() {
	if (this->forwardBackward < 0.0f) {
		return (-this->speedLeftRight / 5);
	}
	else {
		return (this->speedLeftRight / 5);
	}
}

bool Duck::checkCollision() {
	for (unsigned int i = 0; i < this->obstacleModels->size(); ++i) {
		/*std::cout << "Obstacle: " 
			<< this->obstacleModels->at(i)->transform().translation().Z 
			<< " - Duck: "
			<< this->transform().translation().Z
			<< std::endl;*/
		
	}
	return false;
}

bool Duck::cubeIntersection() {

	return false;
}
