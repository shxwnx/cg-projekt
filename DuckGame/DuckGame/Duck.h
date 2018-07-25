//
//  Duck.hpp
//  

#ifndef Duck_hpp
#define Duck_hpp

#include <stdio.h>
#include "Model.h"

class Duck : public BaseModel
{
public:
	Duck();
	virtual ~Duck();
	bool loadModel(const char* file);
	void steer(float forwardBackward, float leftRight);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

protected:
	Model* model;

	float slope;         
	float speedLeftRight;
	float speedForwardBackward;
	float forwardBackward;
	float leftRight;

	float calculateSpeed(float maxSpeed, float currentSpeed, float directionValue, float translation, float border);
	float calculateSlope();
};

#endif /* Duck_hpp */
