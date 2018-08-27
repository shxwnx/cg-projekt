#include "Water.h"
#define PI 3.14159265358979323846

Water::Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ)
{
	this->sizeX = sizeX;
	this->sizeZ = sizeZ;
	this->segmentsX = segmentsX;
	this->segmentsZ = segmentsZ;



	//default wave sollte sich mit den Objekten bewegen
	//ggf. speed erhöhen, wenn Objekte schneller werden
	Wave* defaultWave = new Wave(0.0f, 2.0f, 1.0f, 0.25f, Vector2D(1.0f, 1.0f));
	this->waves.push_back(defaultWave);

	this->time = 0.0f;
}

Water::~Water()
{
	this->testModel = NULL;
	//this->model = NULL;
}

bool Water::loadModel() {
	//this->model->shader(this->pShader, false);
	//this->model = new TrianglePlaneModel(this->sizeX, this->sizeZ, this->segmentsX, this->segmentsZ);
	this->testModel = new LinePlaneModel(this->sizeX, this->sizeZ, this->segmentsX, this->segmentsZ);
	this->testModel->shader(this->pShader, true);
	//Matrix position;
	//position.translation(0, 0, 0);
	//this->testModel->transform(position);*/
	//this->testModel->shader(this->pShader, true);

	return true;
}

void Water::update(float dTime) {
	this->time += dTime;
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
	this->testModel->draw(Cam);
	//this->model->draw(Cam);
}

//In den Shader verlagern
Vector Water::calculateVertexPosition(float x, float z, float time) {
	Vector2D horizontalVector = Vector2D(x, z);
	Vector position;

	float tmpX = 0.0f;
	float tmpZ = 0.0f;
	Wave* current;

	//Horizontal Values
	for (unsigned int i = 0; i < this->waves.size(); ++i) {
		current = this->waves.at(i);
		tmpX += (current->steepness * current->amplitude) * current->direction.X * cos((current->direction*current->frequency).dot(horizontalVector) + current->phase * time);
		tmpZ += (current->steepness * current->amplitude) * current->direction.Y * cos((current->direction*current->frequency).dot(horizontalVector) + current->phase * time);
	}
	position.X = x + tmpX;
	position.Z = z + tmpZ;
	
	//Vertical Value
	for (unsigned int i = 0; i < this->waves.size(); ++i) {
		current = this->waves.at(i);
		position.Y += current->amplitude * sin((current->direction*current->frequency).dot(horizontalVector) + (current->phase * time));
	}

	return position;
}

//In den Shader verlagern
Vector Water::calculateVertexNormal(Vector position, float time) {
	Wave* current;
	float wa, s, c, tmp;
	float tmpX = 0.0f;
	float tmpY = 0.0f;
	float tmpZ = 0.0f;
	for (unsigned int i = 0; i < this->waves.size(); ++i) {
		current = this->waves.at(i); 
		Vector direction = Vector(current->direction.X, 0.0f, current->direction.Y);

		wa = current->frequency * current->amplitude;
		tmp = (direction*current->frequency).dot(position) + current->phase * time;
		s = sin(tmp);
		c = cos(tmp);
		
		tmpX += current->direction.X * wa * c;
		tmpY += current->steepness * wa * s;
		tmpZ += current->direction.Y * wa * c;
	}
	
	return Vector(-tmpX, 1-tmpY, -tmpZ);
}

//http://developer.download.nvidia.com/books/HTML/gpugems/gpugems_ch01.html
//http://fire-face.com/personal/water/index.html
//https://opengllair.wordpress.com/2016/08/24/gerstner-wave-implementation/
//https://www.gamedev.net/forums/topic/648388-gerstner-wave-function-hlsl/

//https://80.lv/articles/tutorial-ocean-shader-with-gerstner-waves/
//https://pastebin.com/Pq8gDFHB
