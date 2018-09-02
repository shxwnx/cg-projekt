#include "Wave.h"
#define PI 3.14159265358979323846

Wave::Wave(): 
	wavelength(0.0f),
	amplitude(0.0f),
	speed(0.0f),
	direction(0.0f, 0.0f, 0.0f)
{
}

Wave::Wave(float wavelength, float amplitude, float speed, Vector direction):
	wavelength(wavelength),
	amplitude(amplitude),
	speed(speed),
	direction(direction)
{
}

Wave::~Wave()
{
}
