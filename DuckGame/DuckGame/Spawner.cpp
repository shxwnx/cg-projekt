#include "Spawner.h"
#include "ToonShader.h"

#include <time.h>
#define PI 3.14159265358979323846

Spawner::Spawner(int countRows, int countObjects, float spacing,
	float spawnTime, float speed, float acceleration, float accelerateTime)
{
	this->defaultSpawnTime = spawnTime;
	this->defaultSpeed = speed;
	this->defaultAcceleration = acceleration;
	this->defaultAccelerateTime = accelerateTime;

	this->countRows = countRows;
	this->countObjects = countObjects;
	this->spacing = spacing;
	this->spawnTime = spawnTime;
	this->speed = speed;
	this->spawnTimePassed = this->speed;
	this->acceleration = acceleration;
	this->accelerateTime = accelerateTime;
	this->accelerateTimePassed = 0.0f;
	this->objectsDodged = 0;
	this->scale = 2.0f;
	this->timePassed = 0.0f;
	this->currentSpeed = 0.0f;
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

			auto model = new Model(file, false, this->scale);
			model->transform(this->defaultTransform());

			auto shader = new ToonShader();
			model->shader(shader, true);

			this->inputModels.push_back(model);
		}
	}
	return true;
}

void Spawner::update(float dtime)
{

	if (!isStopped) {

		this->timePassed += dtime;
		// Spawn Objects
		this->spawnTimePassed += dtime;
		if (this->spawnTimePassed > this->spawnTime) {

			auto random = this->getRandomModel();
			if (random != nullptr) {
				this->inputModels.erase(std::remove(this->inputModels.begin(), this->inputModels.end(), random), this->inputModels.end());

				random->setRotation(this->randomRotation());
				random->transform(this->randomTransform());
				this->outputModels.push_back(random);
			}
			this->rearrange();
			this->spawnTimePassed = 0.0f;
		}

		// Move Objects down
		for (auto model : this->outputModels) {

			auto toonShader = dynamic_cast<ToonShader *>(model->shader());
			toonShader->addTime(dtime);

			Matrix mRotation;
			Matrix mPosition;

			mRotation.rotationY(model->getRotation() * dtime);
			mPosition.translation(0, 0, 2 * this->speed * dtime);

			model->transform(mPosition *  model->transform() * mRotation);
		}

		// Acceleration
		this->accelerateTimePassed += dtime;
		if (this->accelerateTimePassed > this->accelerateTime) {
			this->accelerateTime = 4.0f;
			this->spawnTime -= this->spawnTime * this->acceleration;
			this->accelerateTimePassed = 0.0f;
			this->currentSpeed = this->speed;
		}
	
			this->calculateSpeed();
		
	}
}

void Spawner::calculateSpeed()
{
	if (this->currentSpeed > 0.0f) {
		float maxSpeed = this->currentSpeed * this->acceleration + this->currentSpeed;
		if (this->speed < maxSpeed) {		
			this->speed += this->currentSpeed * this->acceleration / 1000.0f;
		}
	}
}

void Spawner::draw(const BaseCamera & camera)
{
	if (!isStopped) {
		for (auto model : this->outputModels) {
			model->draw(camera);
		}
	}
}

void Spawner::rearrange()
{
	std::vector<Model *> tmp;
	for (auto model : this->outputModels) {
		tmp.push_back(model);
	}
	this->outputModels.clear();
	for (auto model : tmp) {
		auto pos = model->transform().translation();
		if (this->camera != nullptr) {
			if (pos.Z < this->camera->position().Z) {
				this->outputModels.push_back(model);
			}
			else {
				model->transform(this->defaultTransform());
				this->inputModels.push_back(model);
				this->objectsDodged++;
			}
		}

	}
}

Matrix Spawner::defaultTransform()
{
	Matrix position;
	Matrix scale;
	position.translation(0, 0, -9.0f);
	scale.scale(this->scale);
	return position * scale;
}

float Spawner::random(float min, float max) {
	return  (max - min) * ((((float)rand()) / (float)RAND_MAX)) + min;
}

float Spawner::randomRotation()
{
	float rSign = this->random(0, 1);
	int sign = rSign >= 0.5f ? 1 : -1;

	float rot = this->random(0, PI);
	return sign * rot;
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
	if (inputModels.size() > 0) {
		std::random_device randomDevice;
		std::mt19937 engine{ randomDevice() };
		std::uniform_int_distribution<int> dist(0, this->inputModels.size() - 1);

		return inputModels[dist(engine)];
	}
	return nullptr;
}


std::vector<Model*> * Spawner::getOutputModels() {
	return &this->outputModels;
}

void Spawner::reset()
{
	this->timePassed = 0.0f;
	this->spawnTimePassed = 0.0f;
	this->accelerateTimePassed = 0.0f;
	this->spawnTime = this->defaultSpawnTime;
	this->speed = this->defaultSpeed;
	this->currentSpeed = 0.0f;
	this->acceleration = this->defaultAcceleration;
	this->accelerateTime = this->defaultAccelerateTime;

	std::vector<Model*> tmp;
	for (auto model : this->outputModels) {
		tmp.push_back(model);
	}
	this->outputModels.clear();

	for (auto model : tmp) {
		//tmp.erase(std::remove(tmp.begin(), tmp.end(), model), tmp.end());
		model->transform(this->defaultTransform());
		this->inputModels.push_back(model);
	}
	tmp.clear();

}

void Spawner::stop()
{
	this->isStopped = true;
}

void Spawner::start()
{
	this->objectsDodged = 0.0f;
	this->isStopped = false;
}

bool Spawner::getIsStopped() {
	return this->isStopped;
}

