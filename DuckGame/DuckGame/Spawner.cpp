#include "Spawner.h"
#define PI 3.14159265358979323846

Spawner::Spawner(int countRows, int countObjects, float spacing,
	float spawnTime, float speed, float acceleration, float accelerateTime)
{
	this->countRows = countRows;
	this->countObjects = countObjects;
	this->spacing = spacing;
	this->spawnTime = spawnTime;
	this->speed = speed;
	this->spawnTimePassed = this->speed;
	this->acceleration = acceleration;
	this->accelerateTime = accelerateTime;
	this->accelerateTimePassed = 0.0f;
}

Spawner::~Spawner()
{
	this->inputModels.clear();
	this->outputModels.clear();
}

bool Spawner::loadModels(std::vector<const char*> files)
{
	if (files.size() <= 0 || this->countObjects <= 0)
		return false;

	int countPerObject = this->countObjects / files.size();
	for (auto file : files) {
		for (int i = 0; i < countPerObject; i++) {
			auto model = new Model(file, false);

			if (!model->load(file, false)) {
				return false;
			}
			model->shader(this->pShader, false);
			model->transform(this->defaultTransform());
			this->inputModels.push_back(model);
		}
	}
	return true;
}

void Spawner::update(float dtime)
{
	// Spawn Objects
	this->spawnTimePassed += dtime;
	if (this->spawnTimePassed > this->spawnTime) {

		auto random = this->getRandomModel();
		this->inputModels.erase(std::remove(this->inputModels.begin(), this->inputModels.end(), random), this->inputModels.end());
		random->transform(this->randomTransform());
		this->outputModels.push_back(random);
		this->rearrange();
		/*if (this->outputModels.size() >= this->inputModels.size()) {
			auto first = this->outputModels.front();
			this->outputModels.erase(this->outputModels.begin());

			first->transform(this->defaultTransform());
			this->inputModels.push_back(first);
		}*/
		this->spawnTimePassed = 0.0f;
	}

	// Move Objects down
	for (auto model : this->outputModels) {
		Matrix newPosition;
		newPosition.translation(0, 0, this->speed * dtime);
		model->transform(model->transform() * newPosition);
	}

	// Acceleration
	this->accelerateTimePassed += dtime;
	if (this->accelerateTimePassed > this->accelerateTime) {
		this->accelerateTime *= 2;
		this->spawnTime -= this->spawnTime * this->acceleration;
		this->speed += this->speed * this->acceleration;
		this->accelerateTimePassed = 0.0f;
	}
}

void Spawner::draw(const BaseCamera & camera)
{
	for (auto model : this->outputModels) {
		model->draw(camera);
	}
}

void Spawner::rearrange()
{
	for (auto model : this->outputModels) {
		auto pos = model->transform();
		if (pos.m23 > 6.0f) {
				this->outputModels.erase(std::remove(this->outputModels.begin(), this->outputModels.end(), model), this->outputModels.end());
				model->transform(this->defaultTransform());
				this->inputModels.push_back(model);

		}
	}
}

Matrix Spawner::defaultTransform()
{
	Matrix position;
	Matrix scale;
	position.translation(0, 0, -6.0f);
	scale.scale(2);
	return position * scale;
}

Matrix Spawner::randomTransform()
{
	std::random_device randomDevice;
	std::mt19937 engine(randomDevice());
	std::uniform_int_distribution<int> dist(-(int)(this->countRows * this->spacing / 2.0f),
		(int)(this->countRows*this->spacing / 2.0f));
	int range = dist(engine);

	Matrix defaultPosition = this->defaultTransform();
	Matrix randomPosition;
	randomPosition.translation(range, 0, 0);
	return  randomPosition * defaultPosition;
}



Model * Spawner::getRandomModel()
{
	std::random_device randomDevice;
	std::mt19937 engine{ randomDevice() };
	std::uniform_int_distribution<int> dist(0, this->inputModels.size() - 1);
	if (inputModels.size() > 0) {
		return inputModels[dist(engine)];
	}
	return nullptr;
}


