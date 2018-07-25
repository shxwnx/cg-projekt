#include "Spawner.h"
#define PI 3.14159265358979323846

Spawner::Spawner(int count, float spacing, float speed)
{
	this->count = count;
	this->spacing = spacing;
	this->speed = speed;
	this->timePassed = 0.0f;
}

Spawner::~Spawner()
{
	this->inputModels.clear();
	this->outputModels.clear();
}

bool Spawner::loadModels(std::vector<const char*> files)
{
	Matrix mPosition;
	mPosition.translation(0, 0, -4.0f);
	for (auto file : files) {

		auto model = new Model(file, false);

		if (!model->load(file, false)) {
			return false;
		}
		model->shader(this->pShader, false);
		model->transform(mPosition);
		this->inputModels.push_back(model);
	}
	return true;
}

void Spawner::update(float dtime)
{
	float spawnSpeed = this->speed * dtime;
	this->timePassed += spawnSpeed;
	if (this->timePassed > 1.0f) {
		auto model = getRandomModel();
		this->outputModels.push_back(model);
	}
}

void Spawner::draw(const BaseCamera & camera)
{
	for (auto model : this->outputModels) {
		model->draw(camera);
	}
}

Model * Spawner::getRandomModel()
{
	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(0, this->inputModels.size() - 1);
	return inputModels[dist(engine)];
}

