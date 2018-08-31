#pragma once
//
// Water.hpp
//
#define MAX_WAVES 5

#ifndef Water_hpp
#define Water_hpp

#include <math.h>
#include <assimp/vector2.h>
#include "Model.h"
#include "TrianglePlaneModel.h"
#include "LinePlaneModel.h"
#include "Vector2D.h"
#include "Wave.h"
#include "WaterShader.h"

class Water : public BaseModel
{
public:
	Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ);
	virtual ~Water();

	bool loadModel();
	void update(float dTime);
	virtual void draw(const BaseCamera& camera);
	bool generateWaves();
	Vector2D getSize() { return Vector2D(this->sizeX, this->sizeZ); }

protected:
	TrianglePlaneModel* model;
	LinePlaneModel* testModel;

	float sizeX;
	float sizeZ;
	int segmentsX;
	int segmentsZ;

	float time;
	Wave waves[MAX_WAVES];
	//std::vector<Wave*> waves;

	
};

#endif