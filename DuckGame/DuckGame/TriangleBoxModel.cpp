//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth)
{
	float minX = -Width / 2;
	float minY = -Height / 2;
	float minZ = -Depth / 2;

	float maxX = Width / 2;
	float maxY = Height / 2;
	float maxZ = Depth / 2;

	Vector v1, v2, normal;

	// 1. setup vertex buffer
	VB.begin();

		//normal 1 (back)
	v1 = Vector(maxX, minY, minZ) - Vector(maxX, maxY, minZ);
	v2 = Vector(minX, maxY, minZ) - Vector(maxX, maxY, minZ);
	normal = v1.cross(v2).normalize();

		//0
	VB.addNormal(normal);
	VB.addTexcoord0(0, 0);
	VB.addVertex(minX, maxY, minZ);
		//1
	VB.addNormal(normal);
	VB.addTexcoord0(1, 0);
	VB.addVertex(maxX, maxY, minZ);
		//2
	VB.addNormal(normal);
	VB.addTexcoord0(1, 1);
	VB.addVertex(maxX, minY, minZ);
		//3
	VB.addNormal(normal);
	VB.addTexcoord0(0, 1);
	VB.addVertex(minX, minY, minZ);

		//normal 2 (front)
	v1 = Vector(minX, minY, maxZ) - Vector(minX, maxY, maxZ);
	v2 = Vector(maxX, maxY, maxZ) - Vector(minX, maxY, maxZ);
	normal = v1.cross(v2).normalize();

		//4
	VB.addNormal(normal);
	VB.addTexcoord0(0, 1);
	VB.addVertex(maxX, maxY, maxZ);
		//5
	VB.addNormal(normal);
	VB.addTexcoord0(1, 1);
	VB.addVertex(minX, maxY, maxZ);
		//6
	VB.addNormal(normal);
	VB.addTexcoord0(1, 0);
	VB.addVertex(minX, minY, maxZ);
		//7
	VB.addNormal(normal);
	VB.addTexcoord0(0, 0);
	VB.addVertex(maxX, minY, maxZ);

		//normal 3 (left)
	v1 = Vector(minX, minY, minZ) - Vector(minX, maxY, minZ);
	v2 = Vector(minX, maxY, maxZ) - Vector(minX, maxY, minZ);
	normal = v1.cross(v2).normalize();

		//8
	VB.addNormal(normal);
	VB.addTexcoord0(0, 0);
	VB.addVertex(minX, maxY, maxZ);
		//9
	VB.addNormal(normal);
	VB.addTexcoord0(1, 0);
	VB.addVertex(minX, maxY, minZ);
		//10
	VB.addNormal(normal);
	VB.addTexcoord0(1, 1);
	VB.addVertex(minX, minY, minZ);
		//11
	VB.addNormal(normal);
	VB.addTexcoord0(0, 1);
	VB.addVertex(minX, minY, maxZ);

		//normal 4 (right)
	v1 = Vector(maxX, minY, maxZ) - Vector(maxX, maxY, maxZ);
	v2 = Vector(maxX, maxY, minZ) - Vector(maxX, maxY, maxZ);
	normal = v1.cross(v2).normalize();

		//12
	VB.addNormal(normal);
	VB.addTexcoord0(0, 0);
	VB.addVertex(maxX, maxY, minZ);
		//13
	VB.addNormal(normal);
	VB.addTexcoord0(1, 0);
	VB.addVertex(maxX, maxY, maxZ);
		//14
	VB.addNormal(normal);
	VB.addTexcoord0(1, 1);
	VB.addVertex(maxX, minY, maxZ);
		//15
	VB.addNormal(normal);
	VB.addTexcoord0(0, 1);
	VB.addVertex(maxX, minY, minZ);

		//normal 5 (top)
	v1 = Vector(minX, maxY, maxZ) - Vector(minX, maxY, minZ);
	v2 = Vector(maxX, maxY, minZ) - Vector(minX, maxY, minZ);
	normal = v1.cross(v2).normalize();

		//16
	VB.addNormal(normal);
	VB.addTexcoord0(0, 0);
	VB.addVertex(maxX, maxY, minZ);
		//17
	VB.addNormal(normal);
	VB.addTexcoord0(1, 0);
	VB.addVertex(minX, maxY, minZ);
		//18
	VB.addNormal(normal);
	VB.addTexcoord0(1, 1);
	VB.addVertex(minX, maxY, maxZ);
		//19
	VB.addNormal(normal);
	VB.addTexcoord0(0, 1);
	VB.addVertex(maxX, maxY, maxZ);

		//normal 6 (bottom)
	v1 = Vector(minX, minY, minZ) - Vector(minX, minY, maxZ);
	v2 = Vector(maxX, minY, maxZ) - Vector(minX, minY, maxZ);
	normal = v1.cross(v2).normalize();

		//20
	VB.addNormal(normal);
	VB.addTexcoord0(0, 0);
	VB.addVertex(maxX, minY, maxZ);
		//21
	VB.addNormal(normal);
	VB.addTexcoord0(1, 0);
	VB.addVertex(minX, minY, maxZ);
		//22
	VB.addNormal(normal);
	VB.addTexcoord0(1, 1);
	VB.addVertex(minX, minY, minZ);
		//23
	VB.addNormal(normal);
	VB.addTexcoord0(0, 1);
	VB.addVertex(maxX, minY, minZ);

	VB.end();

	// 2. setup index buffer
	unsigned int idx = 0;
	IB.begin();
	for (; idx < 23; idx += 4) {
		//first triangle
		IB.addIndex(idx);
		IB.addIndex(idx + 1);
		IB.addIndex(idx + 2);
		//second triangle
		IB.addIndex(idx);
		IB.addIndex(idx + 2);
		IB.addIndex(idx + 3);
	}
	IB.end();
}

void TriangleBoxModel::draw(const BaseCamera& Cam)
{
	BaseModel::draw(Cam);

	VB.activate();
	IB.activate();

	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);

	IB.deactivate();
	VB.deactivate();
}
