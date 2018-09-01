#pragma once 
#include <stdio.h>
#include "Model.h"

class Pool : public BaseModel
{
public:
	Pool(Camera * cam);
	virtual ~Pool();
	bool loadModel(const char* file);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

protected:
	Model * model;
	Camera* camera;
	float scale; 

};
