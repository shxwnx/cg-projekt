#include "Wave.h"
#define PI 3.14159265358979323846

Wave::Wave() {
	this->steepness = 0.0f;
	this->wavelength = 0.0f;
	this->amplitude = 0.0f;
	this->speed = 0.0f;
	this->direction = Vector2D(0.0f, 0.0f);

	this->frequency = 0.0f;
	this->phase = 0.0f;
}

Wave::Wave(float steepness, float wavelength, float amplitude, float speed, Vector2D direction)
{
	this->steepness = steepness;
	this->wavelength = wavelength;
	this->amplitude = amplitude;
	this->speed = speed;
	this->direction = direction;

	this->frequency = ((2 * PI) / this->wavelength);
	this->phase = this->speed * this->frequency;
}

Wave::~Wave()
{
}
