#pragma once

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <random>
#include "Model.h"

class Spawner : public BaseModel
{
public:
	Spawner(int countRows = 1, int countObjects = 10, float spacing = 1.0f, float spawnTime = 1.0f,
		float speed = 1.0f, float acceleration = 0.1f, float accelerateTime = 1.0f);
	virtual ~Spawner();
	bool loadModels(std::vector<const char*> files);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

	float getSpawnTime() { return this->spawnTime; }
	float getSpeed() { return this->speed; }
	int getObjectsDodged() { return this->objectsDodged; }

	std::vector<Model*> * getOutputModels();
	bool getIsStopped();
	void setCamera(Camera* camera) { this->camera = camera; };
	void reset();
	void stop();
	void start();

protected:
	std::vector<Model*> inputModels;
	std::vector<Model*> outputModels;
	Camera* camera = nullptr;

	int countRows;
	int countObjects;
	float spacing;
	float defaultSpawnTime;
	float defaultSpeed;
	float defaultAcceleration;
	float defaultAccelerateTime;
	float spawnTime; //UI
	float speed; //UI
	float acceleration;
	float accelerateTime;
	int objectsDodged; //UI
	bool isStopped;

private:
	float timePassed;
	float scale;
	float spawnTimePassed;
	float accelerateTimePassed;
	Model* getRandomModel();
	Matrix defaultTransform();
	Matrix randomTransform();
	float random(float min, float max);
	float randomRotation();

	void rearrange();
};

