#pragma once
class Vector2D
{
public:
	float X;
	float Y;

	Vector2D(float x, float y);
	Vector2D();

	float dot(const Vector2D& v) const;
	float cross(const Vector2D& v) const;
	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator=(const Vector2D& v);
	Vector2D operator*(float c) const;
	Vector2D operator/(float c) const;
	Vector2D operator-() const;
	Vector2D& normalize();
	float length() const;
	float lengthSquared() const;
};

