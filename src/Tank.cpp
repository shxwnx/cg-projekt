//
//  Tank.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 16.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Tank.h"

#define PI 3.14159265358979323846

Tank::Tank()
{
	this->speed = 0.0;
	this->rotation = 0.0;
	this->angle_cannon = 0.0;
}

Tank::~Tank()
{
	this->cannonModel = NULL;
	this->chassisModel = NULL;
}

bool Tank::loadModels(const char* ChassisFile, const char* CannonFile)
{
	this->chassisModel = new Model(ChassisFile, false);
	this->cannonModel = new Model(CannonFile, false);

	if (!chassisModel->load(ChassisFile, false)){
		return false;
	}
	if (!cannonModel->load(CannonFile, false)){
		return false;
	}

	this->chassisModel->shader(this->pShader, false);
	this->cannonModel->shader(this->pShader, false);

	return true;
}

void Tank::steer( float ForwardBackward, float LeftRight)
{
	this->speed = ForwardBackward;
	this->rotation = LeftRight;
}

void Tank::aim(const Vector& Target)
{
	Vector v = Target - this->chassisModel->transform().translation();
	v.normalize();
	if (v.Z > 0) //Fallunterscheidung: über und unter dem Einheitskreis
		this->angle_cannon = -acosf(v.X);
	else
		this->angle_cannon = acosf(v.X);
}

void Tank::update(float dtime)
{
	Matrix mat;		//Translation 
	Matrix mat2;	//Rotation
	Matrix mat3;	//Rotation
	Matrix mat4;	//Translation

	mat.translation(1.5*this->speed * dtime, 0, 0);		//Bewegung: pro Sekunde 1.5 Einheiten bei Tastendruck
	mat2.rotationY(PI / 4 * this->rotation * dtime);	//Rotation: pro Sekunde Pi/4 Einheiten bei Tastendruck
	this->chassisModel->transform(this->chassisModel->transform() * mat * mat2);

	mat3.rotationY(this->angle_cannon);					//Rotation: berechneter Angle 
	mat4.translation(this->chassisModel->transform().translation());
	this->cannonModel->transform(mat4  * mat3);
}

void Tank::draw(const BaseCamera& Cam)
{
	this->chassisModel->draw(Cam);
	this->cannonModel->draw(Cam);
}
