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
	Spawner(int countRows = 1, int countObjects = 10, float spacing = 1.0f,
		float spawnTime = 1.0f, float speed = 1.0f, float acceleration = 0.1f, float accelerateTime = 1.0f);
	virtual ~Spawner();
	bool loadModels(std::vector<const char*> files);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

protected:
	std::vector<Model*> inputModels;
	std::vector<Model*> outputModels;

	int countRows;
	int countObjects;
	float spacing;
	float spawnTime;
	float speed;
	float acceleration;
	float accelerateTime;

private:
	float spawnTimePassed;
	float accelerateTimePassed;
	Model* getRandomModel();
	Matrix defaultTransform();
	Matrix randomTransform();
	void rearrange();
};

