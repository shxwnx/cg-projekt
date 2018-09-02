#pragma once 
#include <stdio.h>
#include "Model.h"
#include "ToonShader.h"
#include "Spawner.h"

class Pool : public BaseModel
{
public:
	Pool(Spawner* spawner);
	virtual ~Pool();
	bool loadModel(const char* file);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

protected:
	Model * model;
	Spawner* spawner;
	float scale; 

};
