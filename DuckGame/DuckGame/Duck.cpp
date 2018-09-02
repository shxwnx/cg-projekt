#include "Duck.h"
#include "ToonShader.h"
#include <iostream>

#define PI 3.14159265358979323846
#define FORWARDBACKWARD true
#define LEFTRIGHT false

const float maxX = 5.6f;
const float maxZ = 1.0f;
const float maxSpeedLeftRight = 1.5f;
const float maxSpeedForwardBackward = 0.5f;

Duck::Duck(std::vector<Model*> *obstacleModels, Camera * cam)
{
	this->slope = 0.0;
	this->forwardBackward = 0.0;
	this->speedForwardBackward = 0.0;
	this->leftRight = 0.0;
	this->speedLeftRight = 0.0;
	this->obstacleModels = obstacleModels;
	this->camera = cam;
	this->isCollisionDetected = false;
	this->scale = 4.0f;
}

Duck::~Duck()
{
	this->model = NULL;
}

bool Duck::loadModel(const char* file)
{
	this->model = new Model(file, false, this->scale);
	Matrix default = defaultTransform();
	this->defaultPosition = default.translation();
	this->model->transform(default);

	this->model->shader(this->pShader, true);
	this->setCameraPosition();

	return true;
}

void Duck::steer(float ForwardBackward, float LeftRight)
{
	this->forwardBackward = ForwardBackward;
	this->leftRight = LeftRight;
}

void Duck::update(float dtime)
{
	auto toonShader = dynamic_cast<ToonShader *>(this->shader());
	toonShader->addTime(dtime);

	// movement
	Matrix forwardBackwardMatrix;
	Matrix leftRightMatrix;

	this->speedForwardBackward = this->calculateSpeed(maxSpeedForwardBackward, this->speedForwardBackward, this->forwardBackward, this->model->transform().translation().Z, maxZ, this->defaultPosition.Z);
	forwardBackwardMatrix.translation(0, 0, -this->speedForwardBackward * dtime);

	this->speedLeftRight = this->calculateSpeed(maxSpeedLeftRight, this->speedLeftRight, this->leftRight, this->model->transform().translation().X, maxX, this->defaultPosition.X);
	leftRightMatrix.translation(-this->speedLeftRight * dtime, 0, 0);

	// slope
	Matrix newSlope;
	Matrix oldSlope;
	float newSlopeValue = this->calculateSlope(maxX);

	newSlope.rotationY(newSlopeValue);
	oldSlope.rotationY(-this->slope);

	// transform
	this->model->transform(this->model->transform() * oldSlope * forwardBackwardMatrix * leftRightMatrix * newSlope);
	this->slope = newSlopeValue;

	// camera positon
	this->setCameraPosition();

	// check collision
	this->checkCollision(dtime);
}

void Duck::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}

bool Duck::collisionDetected()
{
	return this->isCollisionDetected;
}

float Duck::calculateSpeed(float maxSpeed, float currentSpeed, float directionValue, float translation, float border, float defaultTranslation) 
{
	float speed = 0.0f;
	// no border
	if ((translation < defaultTranslation + border) && (translation > defaultTranslation - border)) { 
		// no key pressed
		if (directionValue == 0.0f) {												
			if (currentSpeed == 0.0f) {
				speed = 0.0f;
			}
			else if (currentSpeed < 0.0f) {
				speed = currentSpeed + maxSpeed / 10;
			}
			else if (currentSpeed > 0.0f) {
				speed = currentSpeed - maxSpeed / 10;
			}

		}
		// right pressed
		else if (directionValue < 0.0f && -maxSpeed < currentSpeed) {				
			speed = currentSpeed - maxSpeed / 10;
		}
		// left pressed
		else if (directionValue > 0.0f && maxSpeed > currentSpeed) {				
			speed = currentSpeed + maxSpeed / 10;
		}
		else {
			speed = currentSpeed;
		}
	}
	// left border and right pressed
	else if (translation < defaultTranslation + border && directionValue < 0.0f) {	
		if (-maxSpeed < currentSpeed) {
			speed = currentSpeed - maxSpeed / 10;
		}
		else {
			speed = currentSpeed;
		}
	}
	// right border and left pressed
	else if (translation > defaultTranslation - border && directionValue > 0.0f) {	
		if (maxSpeed > currentSpeed) {
			speed = currentSpeed + maxSpeed / 10;
		}
		else {
			speed = currentSpeed;
		}
	}

	return speed;
}

float Duck::calculateSlope(float border) 
{
	// border
	if (this->model->transform().translation().X >= this->defaultPosition.X + border || this->model->transform().translation().X <= this->defaultPosition.X - border) { 
		if (this->slope > 0.1f) {
			return this->slope - 0.1;
		}
		else if (this->slope < -0.1f) {
			return this->slope + 0.1;
		}
		else return 0.0f;
	}
	// no border and backward
	else if (this->forwardBackward < 0.0f) {	
		return (-this->speedLeftRight / 5);
	}
	// no border and forward
	else if (this->forwardBackward > 0.0f) {	
		return (this->speedLeftRight / 5);
	}
	// no border and neither for- or backward
	else {							
		// no movement
		if (this->leftRight == 0.0) {	
			if (this->slope > 0.1f) {
				return this->slope - 0.1;
			}
			else if (this->slope < -0.1f) {
				return this->slope + 0.1;
			}
			else return 0.0f;
		}
		// only sideways
		else {							
			return (this->speedLeftRight / 5);
		}
		
	}
}

Matrix Duck::defaultTransform() 
{
	Matrix mPosition;
	Matrix mScale;
	mPosition.translation(0.0f, -0.2f, 9.2f);
	mScale.scale(this->scale);
	return mPosition * mScale;
}

void Duck::setCameraPosition() 
{
	Vector actualCameraPosition = this->camera->position();
	Vector cameraPositon(this->model->transform().translation().X, actualCameraPosition.Y, actualCameraPosition.Z);
	Vector cameraTarget(this->model->transform().translation().X, this->model->transform().translation().Y, this->defaultTransform().translation().Z - maxZ + (this->model->transform().translation().Z - this->defaultTransform().translation().Z)*0.2);

	this->camera->setPosition(cameraPositon);
	this->camera->setTarget(cameraTarget);
}

void Duck::reset() 
{
	this->slope = 0.0f;
	this->speedLeftRight = 0.0f;
	this->speedForwardBackward = 0.0f;
	this->forwardBackward = 0.0f;
	this->leftRight = 0.0f;

	this->model->transform(defaultTransform());
	setCameraPosition();
	this->isCollisionDetected = false;
}

void Duck::checkCollision(float dtime) 
{
	static float timePassed = 0.0f;
	timePassed += dtime;
	if (timePassed > 0.1f) {
		for (auto object : (*this->obstacleModels)) {
			if (this->model->transform().translation().X - 0.7 < object->transform().translation().X
				&& this->model->transform().translation().X + 0.7 > object->transform().translation().X) {

				if (this->model->transform().translation().Z - 0.8f < object->transform().translation().Z
					&& this->model->transform().translation().Z + 0.5f > object->transform().translation().Z) {

					timePassed = 0.0f;
					this->isCollisionDetected = true;
					/*if (this->boundingBoxIntersection(object)) {
						timePassed = 0.0f;
						this->isCollisionDetected = true;
					}*/
				}
			}
		}
	}


}

bool Duck::boundingBoxIntersection(const Model* object) 
{
	// box Collision
	Vector posDuck = this->model->transform().translation();
	Vector posObject = object->transform().translation();

	AABB modelBox = this->model->boundingBox();
	AABB objectBox = object->boundingBox();

	if (modelBox.getX() - objectBox.getX() < modelBox.getSizeX() + objectBox.getSizeX()) {
		if (modelBox.getZ() - objectBox.getZ() < modelBox.getSizeZ() + objectBox.getSizeZ()) {
			if (modelBox.getY() - objectBox.getY() < modelBox.getSizeY() + objectBox.getSizeY()) {
				return false;
			}
		}
	}
	return false;
}
