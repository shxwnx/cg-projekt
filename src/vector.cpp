#include "vector.h"
#include <assert.h>
#include <math.h>

Vector::Vector( float x, float y, float z): X(x), Y(y), Z(z)
{

}

Vector::Vector()
{

}

float Vector::dot(const Vector& v) const
{
	float f = X*v.X + Y*v.Y + Z*v.Z;
	return f;
}

Vector Vector::cross(const Vector& v) const
{
	float tempX = Y*v.Z - v.Y*Z;
	float tempY = -(X*v.Z - v.X*Z);
	float tempZ = X*v.Y - v.X*Y;

	return Vector(tempX, tempY, tempZ);
}

Vector Vector::operator+(const Vector& v) const
{
	float tempX = X + v.X;
	float tempY = Y + v.Y;
	float tempZ = Z + v.Z;

	return Vector(tempX, tempY, tempZ);
}

Vector Vector::operator-(const Vector& v) const
{
	float tempX = X - v.X;
	float tempY = Y - v.Y;
	float tempZ = Z - v.Z;

	return Vector(tempX, tempY, tempZ);
}

Vector Vector::operator*(float c) const
{
	float tempX = c*X;
	float tempY = c*Y;
	float tempZ = c*Z;

	return Vector(tempX, tempY, tempZ);
}

Vector Vector::operator-() const
{
	return Vector(-X, -Y, -Z);
}

Vector& Vector::operator+=(const Vector& v)
{
	X = X + v.X;
	Y = Y + v.Y;
	Z = Z + v.Z;

	return *this;
}

Vector& Vector::operator=(const Vector& v)
{
	X = v.X;
	Y = v.Y;
	Z = v.Z;

	return *this;
}

Vector& Vector::normalize()
{
	float length = this->length();

	X = X / length;
	Y = Y / length;
	Z = Z / length;

	return *this;
}

float Vector::length() const
{
	return sqrt(this->lengthSquared()); 
}

float Vector::lengthSquared() const
{
	return X*X + Y*Y + Z*Z;
}

Vector Vector::reflection( const Vector& normal) const
{
	float tmp = -2*(this->dot(normal));
	Vector tmpV = (normal)*tmp + (*this);
	return tmpV;
}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
	
	//Vector n = ((b - a).cross(c - a))/ ((b - a).cross(c - a)).length();

	//Schnittpunkt mit der Ebene bestimmen
	Vector edge1 = b - a;
	Vector edge2 = c - a;
	Vector n = edge1.cross(edge2).normalize();

	s = ((a - *(this)).dot(n)) / (d.dot(n));
	if (s < 0) {
		return false;
	}

	Vector p = *(this) + d*s;

	//Prüfen, ob Schnittpunkt p mit Ebene innerhalb des Dreiecks liegt
	Vector v1 = a - p;
	Vector v2 = b - p;
	Vector v3 = c - p;

	float area = edge1.cross(edge2).length() / 2;
	float cmparea1 = v1.cross(v2).length() / 2;
	float cmparea2 = v2.cross(v3).length() / 2;
	float cmparea3 = v1.cross(v3).length() / 2;


	if (area+0.000001f >= cmparea1 + cmparea2 + cmparea3) {
		return true;
	}
	else {
		return false;
	}
}
