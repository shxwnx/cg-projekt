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
	bool loadModel(const char* DuckFile);
	void steer(float ForwardBackward, float LeftRight);
	void update(float dtime);
	virtual void draw(const BaseCamera& Cam);

protected:
	Model* duckModel;

	float rotation;         //ggf. Neigung in der Kurve
	float forwardBackward;
	float leftRight;
};

#endif /* Duck_hpp */
