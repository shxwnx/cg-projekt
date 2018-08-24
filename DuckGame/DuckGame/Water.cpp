#include "Water.h"

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

Vector Water::calculateVertexPosition(int x, int z, float time) {
	return Vector();
}
