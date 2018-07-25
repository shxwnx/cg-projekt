#include "Spawner.h"
#define PI 3.14159265358979323846

Spawner::Spawner(int count, float spacing, float speed)
{
	this->count = count;
	this->spacing = spacing;
	this->speed = speed;
}

Spawner::~Spawner()
{
	this->inputModels.clear();
	this->outputModels.clear();
}

bool Spawner::loadModels(std::vector<const char*> files)
{
	for (auto file : files) {

		auto model = new Model(file, false);
		Matrix mPosition;
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
}

void Spawner::draw(const BaseCamera & camera)
{
	for (auto model : this->outputModels) {
		model->draw(camera);
	}
}