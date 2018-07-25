//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel( float Width, float Height, float Depth )
{
	this->VB.begin();

	float minX = -Width / 2.0f;
	float minY = -Height / 2.0f;
	float minZ = -Depth / 2.0f;

	float maxX = Width / 2.0f;
	float maxY = Height / 2.0f;
	float maxZ = Depth / 2.0f;

	VB.addVertex(minX, minY, maxZ);
	VB.addVertex(minX, maxY, maxZ);

	VB.addVertex(minX, maxY, maxZ);
	VB.addVertex(maxX, maxY, maxZ);

	VB.addVertex(maxX, maxY, maxZ);
	VB.addVertex(maxX, minY, maxZ);

	VB.addVertex(maxX, minY, maxZ);
	VB.addVertex(minX, minY, maxZ);


	VB.addVertex(minX, minY, minZ);
	VB.addVertex(minX, maxY, minZ);

	VB.addVertex(minX, maxY, minZ);
	VB.addVertex(maxX, maxY, minZ);

	VB.addVertex(maxX, maxY, minZ);
	VB.addVertex(maxX, minY, minZ);

	VB.addVertex(maxX, minY, minZ);
	VB.addVertex(minX, minY, minZ);


	VB.addVertex(minX, minY, minZ);
	VB.addVertex(minX, minY, maxZ);

	VB.addVertex(minX, maxY, minZ);
	VB.addVertex(minX, maxY, maxZ);

	VB.addVertex(maxX, maxY, minZ);
	VB.addVertex(maxX, maxY, maxZ);

	VB.addVertex(maxX, minY, minZ);
	VB.addVertex(maxX, minY, maxZ);
	
	VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

	VB.activate();

	glDrawArrays(GL_LINES, 0, VB.vertexCount());

	VB.deactivate();
}
