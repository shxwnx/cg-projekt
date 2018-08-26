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
#include "LinePlaneModel.h"
#include "Vector2D.h"
#include "Wave.h"

class Water : public BaseModel
{
public:
	Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ);
	virtual ~Water();

	bool loadModel();
	void update(float dTime);

protected:
	TrianglePlaneModel* model;
	LinePlaneModel* testModel;

	float sizeX;
	float sizeZ;
	int segmentsX;
	int segmentsZ;

	float time;

	std::vector<Wave*> waves;

	Vector calculateVertexPosition(float x, float z, float time);
	Vector calculateVertexNormal(Vector position, float time);
};

#endif