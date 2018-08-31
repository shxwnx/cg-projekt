#include "Spawner.h"
#include "ToonShader.h"
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
			if (!model->load(file, false)) {
				return false;
			}
			auto shader = new ToonShader();
			model->shader(shader, false);

			this->inputModels.push_back(model);
		}
	}
	return true;
}

void Spawner::update(float dtime)
{
	if (!isStopped) {
		// Spawn Objects
		this->spawnTimePassed += dtime;
		if (this->spawnTimePassed > this->spawnTime) {

			auto random = this->getRandomModel();
			if (random != nullptr) {
				this->inputModels.erase(std::remove(this->inputModels.begin(), this->inputModels.end(), random), this->inputModels.end());
				random->transform(this->randomTransform());
				this->outputModels.push_back(random);
			}
			this->rearrange();
			this->spawnTimePassed = 0.0f;
		}

		// Move Objects down
		for (auto model : this->outputModels) {

			Matrix mRotation;
			Matrix mPosition;

			mRotation.rotationY(PI * dtime);
			mPosition.translation(0, 0, 2 * this->speed * dtime);

			model->transform(mPosition *  model->transform() * mRotation);
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
}

void Spawner::draw(const BaseCamera & camera)
{
	if (!isStopped) {
		for (auto model : this->outputModels) {
			model->draw(camera);
		}
	}
}

float Spawner::getSpawnTime()
{
	return this->spawnTime;
}

float Spawner::getSpeed()
{
	return this->speed;
}

int Spawner::getObjectsDodged()
{
	return this->objectsDodged;
}

void Spawner::rearrange()
{
	for (auto model : this->outputModels) {
		auto pos = model->transform();
		if (pos.m23 > 6.0f) {
			this->outputModels.erase(std::remove(this->outputModels.begin(), this->outputModels.end(), model), this->outputModels.end());
			model->transform(this->defaultTransform());
			this->inputModels.push_back(model);
			this->objectsDodged++;
		}
	}
}

Matrix Spawner::defaultTransform()
{
	Matrix position;
	Matrix scale;
	position.translation(0, 0, -6.0f);
	scale.scale(this->scale);
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
	this->spawnTimePassed = 0.0f;
	this->accelerateTimePassed = 0.0f;
	this->spawnTime = this->defaultSpawnTime;
	this->speed = this->defaultSpeed;
	this->acceleration = this->defaultAcceleration;
	this->accelerateTime = this->defaultAccelerateTime;

	std::vector<Model*> tmp;
	for (auto model : this->outputModels) {
		tmp.push_back(model);
	}
	this->outputModels.clear();

	for (auto model : tmp) {
		tmp.erase(std::remove(tmp.begin(), tmp.end(), model), tmp.end());
		model->transform(this->defaultTransform());
		this->inputModels.push_back(model);
	}
	
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

