#include "Water.h"
#define PI 3.14159265358979323846

Water::Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ, Spawner* spawner)
{
	this->sizeX = sizeX;
	this->sizeZ = sizeZ;
	this->segmentsX = segmentsX;
	this->segmentsZ = segmentsZ;

	this->spawner = spawner;

	//default wave sollte sich mit den Objekten bewegen
	//ggf. speed erhöhen, wenn Objekte schneller werden
	//Wave* wave1 = new Wave(0.0f, 1.0f, 0.05f, 1.0f, Vector2D(0.0f, -1.0f));
	//generateWaves();

	//this->waves.push_back(defaultWave);

	this->time = 0.0f;
}

Water::~Water()
{
	//this->testModel = NULL;
	this->model = NULL;
}

bool Water::loadModel() {
	
	this->model = new TrianglePlaneModel(this->sizeX, this->sizeZ, this->segmentsX, this->segmentsZ);
	this->model->shader(this->pShader, true);

	//this->testModel = new LinePlaneModel(this->sizeX, this->sizeZ, this->segmentsX, this->segmentsZ);
	//this->testModel->shader(this->pShader, true);
	//Matrix position;
	//position.translation(0, 0, 0);
	//this->testModel->transform(position);*/
	//this->testModel->shader(this->pShader, true);

	return true;
}

void Water::update(float dTime) {
	this->time += dTime;

	WaterShader* pShader = dynamic_cast<WaterShader*>(this->pShader);
	pShader->setTime(this->time);
	pShader->setSpeed(this->spawner->getSpeed());

	Vector tmp;
	for (float x = 0.0f; x <= this->sizeX; x += (this->sizeX / this->segmentsX)) {
		for (float z = 0.0f; z <= this->sizeZ; z += (this->sizeZ / this->segmentsZ)) {
			//tmp = calculateVertexPosition(x, z, this->time);
			//calculateVertexNormal(tmp, this->time);
		}
	}
	//für jeden Vertex neu berechnen und setzen
}

void Water::draw(const BaseCamera& Cam)
{
	//this->testModel->draw(Cam);
	this->model->draw(Cam);
}

bool Water::generateWaves() {
	Wave* wave1 = new Wave(1.0f, 0.03f, 0.6f, Vector(0.0f, 0.0f, -1.0f));
	Wave* wave2 = new Wave(2.0f, 0.04f, 0.4f, Vector(0.5f, 0.0f, -0.5f));
	Wave* wave3 = new Wave(0.5f, 0.01f, 0.2f, Vector(0.5f, 0.0f, 0.25f));
	Wave* wave4 = new Wave(3.0f, 0.01f, 0.1f, Vector(-1.0f, 0.0f, 0.0f));

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


//http://developer.download.nvidia.com/books/HTML/gpugems/gpugems_ch01.html
//http://fire-face.com/personal/water/index.html
//https://opengllair.wordpress.com/2016/08/24/gerstner-wave-implementation/
//https://www.gamedev.net/forums/topic/648388-gerstner-wave-function-hlsl/

//https://80.lv/articles/tutorial-ocean-shader-with-gerstner-waves/
//https://pastebin.com/Pq8gDFHB
