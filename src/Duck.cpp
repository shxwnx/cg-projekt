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
	this->model= NULL;
}

bool Duck::loadModel(const char* file)
{
	this->model = new Model(file, false);

	if (!this->model->load(file, false)) {
		return false;
	}
	this->model->shader(this->pShader, false);
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
	float maxX = 2.5f; //leftRight Maxima
	float maxZ = 2.5f; //forwardBackward Maxima
	float maxSpeedLeftRight = 2.5f; //maximale leftRight Geschwindigkeit
	float maxSpeedForwardBackward = 1.0f; //maximale forwardBackward Geschwindigkeit

	duckTransform = this->model->transform();

	/*if (this->leftRight == 0.0f) { //Keine Taste gedrückt
		if (this->speedLeftRight > 0.0f) { this->speedLeftRight = 0.0f; }
		if (this->speedLeftRight < 0.0f) { this->speedLeftRight = 0.0f; }
		//An 0 annähern
	}
	else if (this->leftRight < 0.0f && -maxSpeedLeftRight * this->leftRight > this->speedLeftRight) { //Links
		this->speedLeftRight = +(-maxSpeedLeftRight * this->leftRight) / 10;
		//an -maxSpeedLeftRight * this->leftRight annähern
	}
	else if (this->leftRight < 0.0f && -maxSpeedLeftRight * this->leftRight < this->speedLeftRight) { //Rechts
		this->speedLeftRight = +(-maxSpeedLeftRight * this->leftRight) / 10;
		//an -maxSpeedLeftRight * this->leftRight annähern
	}
	std::cout << this->speedLeftRight;*/


	if (duckTransform.translation().X >= maxX && this->leftRight < 0.0f) { //rechte Kante
		mat2.translation(0, 0, 0);
	}
	else if (duckTransform.translation().X <= -maxX && this->leftRight > 0.0f) { //linke Kante
		mat2.translation(0, 0, 0);
	}
	else{
		mat2.translation(-maxSpeedLeftRight * this->leftRight * dtime, 0, 0);
		//mat2.translation(this->speedLeftRight * dtime, 0, 0);
	}

	if (duckTransform.translation().Z >= maxZ && this->forwardBackward < 0.0f) { //obere Kante
		mat1.translation(0, 0, 0);
	}
	else if (duckTransform.translation().Z <= -maxZ && this->forwardBackward > 0.0f) { //untere Kante
		mat1.translation(0, 0, 0);
	}
	else {
		mat1.translation(0, 0, -maxSpeedForwardBackward * this->forwardBackward * dtime);
	}

	//mat1.translation(0 , 0, -1*this->forwardBackward * dtime);
	//mat2.translation(-2*this->leftRight * dtime, 0, 0);
	
	//std::cout << "X: " << duckTransform.translation().X << " || "
		//<< "Z: " << duckTransform.translation().Z << " || ";


	this->model->transform(this->model->transform() * mat1 * mat2);

}

void Duck::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}
