#pragma once
#include "Model.h"
#include "Application.h"
#include "Spawner.h"
#include <GLFW/glfw3.h>

class UIService : public BaseModel {

	UIService();
public:

	UIService(Application application, Spawner spawner);
	virtual ~UIService();
	bool loadUI(std::vector<const char*> files);
	void update(float dtime);
	virtual void draw(const BaseCamera& camera);

private:
	std::vector<Model*> ui;
	// Provider
	Application application;
	Spawner spawner;

	// UI
	float timePassed;
	float spawnTime;
	float speed;
	int objectsDodged;
};