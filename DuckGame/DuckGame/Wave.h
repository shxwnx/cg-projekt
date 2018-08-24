#pragma once
#include "Vector2D.h"

class Wave
{
public:
	Wave();
	Wave(float steepness, float wavelength, float amplitude, float speed, Vector2D direction);
	~Wave();

	float steepness;	//pinch on the wave 
	float wavelength;	//crest-to-crest distance between waves in world space
	float amplitude;	//height from the water plane to the wave crest
	float speed;		//distance the crest moves forward per second
	Vector2D direction;	//horizontal vector perpendicular to the wave front along which the crest travels
	float frequency;
	float phase;
};

