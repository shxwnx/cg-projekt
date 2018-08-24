#include "Vector2D.h"

#include <assert.h>
#include <math.h>

Vector2D::Vector2D(float x, float y) : X(x), Y(y)
{

}

Vector2D::Vector2D()
{

}

float Vector2D::dot(const Vector2D& v) const
{
	float f = X * v.X + Y * v.Y;
	return f;
}

float Vector2D::cross(const Vector2D& v) const
{
	return X * v.Y - v.X * Y;
}

Vector2D Vector2D::operator+(const Vector2D& v) const
{
	float tempX = X + v.X;
	float tempY = Y + v.Y;

	return Vector2D(tempX, tempY);
}

Vector2D Vector2D::operator-(const Vector2D& v) const
{
	float tempX = X - v.X;
	float tempY = Y - v.Y;

	return Vector2D(tempX, tempY);
}

Vector2D Vector2D::operator*(float c) const
{
	float tempX = c * X;
	float tempY = c * Y;

	return Vector2D(tempX, tempY);
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-X, -Y);
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	X = X + v.X;
	Y = Y + v.Y;

	return *this;
}

Vector2D& Vector2D::operator=(const Vector2D& v)
{
	X = v.X;
	Y = v.Y;

	return *this;
}

Vector2D& Vector2D::normalize()
{
	float length = this->length();

	X = X / length;
	Y = Y / length;

	return *this;
}

float Vector2D::length() const
{
	return sqrt(this->lengthSquared());
}

float Vector2D::lengthSquared() const
{
	return X * X + Y * Y;
}
