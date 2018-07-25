#pragma once

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <random>
#include <thread>
#include "Model.h"

class Spawner : public BaseModel
{
public:
	Spawner(int count = 1, float spacing = 1.0f, float spawnTime = 1.0f, float speed = 1.0f);
	virtual ~Spawner();
	bool loadModels( std::vector<const char*> files);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

protected:
	std::vector<Model*> inputModels;
	std::vector<Model*> outputModels;

	int count;
	float spacing;
	float spawnTime;
	float speed;

private:
	float timePassed;
	Model* getRandomModel();
	Matrix defaultTransform();
};

