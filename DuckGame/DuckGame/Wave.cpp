#include "Wave.h"
#define PI 3.14159265358979323846

Wave::Wave() {
	this->wavelength = 0.0f;
	this->amplitude = 0.0f;
	this->speed = 0.0f;
	this->direction = Vector(0.0f, 0.0f, 0.0f);
}

Wave::Wave(float wavelength, float amplitude, float speed, Vector direction)
{
	this->wavelength = wavelength;
	this->amplitude = amplitude;
	this->speed = speed;
	this->direction = direction;
}

Wave::~Wave()
{
}
