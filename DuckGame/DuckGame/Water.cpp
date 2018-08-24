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

void Water::generateWaves(float dTime, int count) {

}

void Water::generateTrianglePlane() {
	
}

Vector Water::calculateWave(float x, float z, float time) {
	Vector2D horizontalVector = Vector2D(x, z);
	float tmp;
	Vector wave;
	//float newX, newY, newZ;
	//float cosinusValue;
	float w, phase;

	w = ((2 * PI) / this->wavelength);
	phase = this->speed * w;

	tmp = (this->steepness * this->amplitude) * this->direction.X * cos((this->direction*w).dot(horizontalVector) + phase * time);
		//(this->direction / w) * this->amplitude * sin(this->direction.dot(horizontalVector) - (phase * time));
	wave.X = x + tmp;
	wave.Z = z + tmp;
	
	wave.Y = this->amplitude * sin((this->direction*w).dot(horizontalVector) + (phase * time));

	return wave;
}
