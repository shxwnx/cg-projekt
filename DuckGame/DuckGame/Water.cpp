#include "Water.h"
#define PI 3.14159265358979323846

Water::Water(float sizeX, float sizeZ, int segmentsX, int segmentsZ)
{
	this->sizeX = sizeX;
	this->sizeZ = sizeZ;
	this->segmentsX = segmentsX;
	this->segmentsZ = segmentsZ;

	this->model = new TrianglePlaneModel(this->sizeX, this->sizeZ, this->segmentsX, this->segmentsZ);
}

Water::~Water()
{
	this->model = NULL;
}

void Water::generateWaves(float dTime) {

}

void Water::generateTrianglePlane() {
	
}

Vector Water::calculateWave(float x, float z, float time) {
	Vector2D horizontalVector = Vector2D(x, z);
	Vector2D tmp;
	Vector wave;
	//float newX, newY, newZ;
	//float cosinusValue;
	float w, phase;

	w = ((2 * PI) / this->wavelength);
	phase = sqrt(this->speed * w);

	tmp = (this->direction / w) * this->amplitude * sin(this->direction.dot(horizontalVector) - (phase * time));
	wave.X = tmp.X;
	wave.Z = tmp.Y;
	
	wave.Y = this->amplitude * cos(this->direction.dot(horizontalVector) - (phase * time)) / 2 - (this->amplitude / 2);

	//(this->steepness * this->amplitude) 
	//cosinusValue = cos((this->direction * w).dot(horizontalVector)) + (phase * time);

	//newY = this->amplitude;
	//this->direction.

	//float tmp = cos((this->wavelength*this->direction)*vertexNumber + this->phase*time);
	//newPositionX = x + (this->direction->this->steepness*this->amplitude 

	return wave;
}
