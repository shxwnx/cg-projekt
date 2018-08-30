#pragma once
#include "Vector.h"

class Wave
{
public:
	Wave();
	Wave(float wavelength, float amplitude, float speed, Vector direction);
	~Wave();

	float wavelength;	//crest-to-crest distance between waves in world space
	float amplitude;	//height from the water plane to the wave crest
	float speed;		//distance the crest moves forward per second
	Vector direction;	//horizontal vector perpendicular to the wave front along which the crest travels
};

