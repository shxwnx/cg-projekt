#pragma once
//
// Water.hpp
//

#ifndef Water_hpp
#define Water_hpp

#include <math.h>
#include <assimp/vector2.h>
#include "Model.h"
#include "TrianglePlaneModel.h"
#include "Vector2D.h"

class Water : public BaseModel
{
public:
	Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ);
	virtual ~Water();

	void generateWaves(float dTime, int count);

protected:
	TrianglePlaneModel* model;

	float sizeX;
	float sizeZ;
	int segmentsX;
	int segmentsZ;

	float steepness;	//Q pinch on the wave 
	float wavelength;	//L crest-to-crest distance between waves in world space
	float amplitude;	//A height from the water plane to the wave crest
	float speed;		//S  distance the crest moves forward per second
	Vector2D direction;	//D horizontal vector perpendicular to the wave front along which the crest travels 
						//not needed for circular waves

	void generateTrianglePlane();
	Vector calculateWave(float x, float z, float time);
};

#endif