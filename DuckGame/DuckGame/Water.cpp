#include "Water.h"
#define PI 3.14159265358979323846

Water::Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ, Spawner* spawner): 
	sizeX(sizeX),
	sizeZ(sizeZ),
	segmentsX(segmentsX),
	segmentsZ(segmentsZ),
	spawner(spawner),
	time(0.0f)
{
}

Water::~Water()
{
	this->model = NULL;
}

bool Water::loadModel() 
{
	this->model = new TrianglePlaneModel(this->sizeX, this->sizeZ, this->segmentsX, this->segmentsZ);
	this->model->shader(this->pShader, true);
	return true;
}

void Water::update(float dTime) {
	this->time += dTime;

	WaterShader* pShader = dynamic_cast<WaterShader*>(this->pShader);
	pShader->setTime(this->time);
	pShader->setSpeed(this->spawner->getSpeed());
}

void Water::draw(const BaseCamera& Cam)
{
	this->model->draw(Cam);
}

bool Water::generateWaves() {
	Wave* wave1 = new Wave(2.0f, 0.03f, 6.0f, Vector(0.0f, 0.0f, -1.0f));
	Wave* wave2 = new Wave(3.0f, 0.02f, 0.4f, Vector(0.5f, 0.0f, -0.5f));
	Wave* wave3 = new Wave(1.0f, 0.01f, 1.0f, Vector(0.5f, 0.0f, 0.0f));
	Wave* wave4 = new Wave(4.0f, 0.02f, 0.5f, Vector(-1.0f, 0.0f, 0.0f));

	this->waves[0] = *wave1;
	this->waves[1] = *wave2;
	this->waves[2] = *wave3;
	this->waves[3] = *wave4;

	if (this->pShader == NULL) return false;

	WaterShader* pShader = dynamic_cast<WaterShader*>(this->pShader);
	if (pShader == NULL) return false;

	pShader->setWaves(this->waves, 4);
	return true;
}

