#pragma once
//
// Water.hpp
//

#ifndef Water_hpp
#define Water_hpp

#include <math.h>
#include "Model.h"
#include "TrianglePlaneModel.h"

class Water : public BaseModel
{
public:
	Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ);
	virtual ~Water();

	void generateWave(float dTime);

protected:
	TrianglePlaneModel* model;

	float sizeX;
	float sizeZ;
	int segmentsX;
	int segmentsZ;

	float wavelength;	//L crest-to-crest distance between waves in world space
	float amplitude;	//A height from the water plane to the wave crest
	float speed;		//S distance the crest moves forward per second
	Vector direction;	//D horizontal vector perpendicular to the wave front along which the crest travels

	void generateTrianglePlane();
	Vector calculateVertexPosition(int x, int z, float time);
};

#endif