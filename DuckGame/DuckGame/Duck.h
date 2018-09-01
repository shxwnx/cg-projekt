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
	Duck(std::vector<Model*> *obstacleModels, Camera * cam);
	virtual ~Duck();
	bool loadModel(const char* file);
	void steer(float forwardBackward, float leftRight);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

	bool collisionDetected();
	void reset();

protected:
	Model * model;
	Camera* camera;
	std::vector<Model*> *obstacleModels;
	float scale; 

	float slope;
	float speedLeftRight;
	float speedForwardBackward;
	float forwardBackward;
	float leftRight;

	bool isCollisionDetected;

	float calculateSpeed(float maxSpeed, float currentSpeed, float directionValue, float translation, float border, bool direction);
	float calculateSlope(float border);

	void checkCollision(float dtime);

private:
	void setCameraPosition();
	Matrix defaultTransfrom();
	bool boundingBoxIntersection(const Model* object);
};

#endif /* Duck_hpp */
