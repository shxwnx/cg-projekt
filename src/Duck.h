//
//  Duck.hpp
//  

#ifndef Duck_hpp
#define Duck_hpp

#include <stdio.h>
#include "model.h"

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

	float rotation;         //ggf. Neigung in der Kurve
	float speedLeftRight;
	float forwardBackward;
	float leftRight;
};

#endif /* Duck_hpp */
