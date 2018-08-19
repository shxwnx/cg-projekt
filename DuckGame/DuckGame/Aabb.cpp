//
//  Aabb.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 02.11.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Aabb.h"

AABB::AABB()
{
    
}
AABB::AABB(const Vector& min, const Vector& max) : Min(min), Max(max)
{
    
}
AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : Min(minX, minY, minZ), Max(maxX, maxY, maxZ)
{
    
}

Vector AABB::size() const
{
    return Max-Min;
}

float AABB::getX() const {
	return (this->Max.X + this->Min.X) / 2;
}

float AABB::getY() const {
	return (this->Max.Y + this->Min.Y) / 2;
}

float AABB::getZ() const {
	return (this->Max.Z + this->Min.Z) / 2;
}

float AABB::getSizeX() const {
	return (this->Max.X - this->Min.X) / 2;
}

float AABB::getSizeY() const {
	return (this->Max.Y - this->Min.Y) / 2;
}

float AABB::getSizeZ() const {
	return (this->Max.Z - this->Min.Z) / 2;
}
