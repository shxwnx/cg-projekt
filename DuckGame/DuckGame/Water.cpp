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

void Water::generateWave(float dTime) {

}

void Water::generateTrianglePlane() {
	
}

Vector Water::calculateVertexPosition(float x, float z, float time) {
	Vector2D* horizontalVector = new Vector2D(x, z);
	float newX, newY, newZ;
	float cosinusValue;

	cosinusValue = ((2 * PI) / this->wavelength);// * this->direction;


	//newY = this->amplitude;
	//this->direction.

	//float tmp = cos((this->wavelength*this->direction)*vertexNumber + this->phase*time);
	//newPositionX = x + (this->direction->this->steepness*this->amplitude 

	return Vector(newX, newY, newZ);
}
