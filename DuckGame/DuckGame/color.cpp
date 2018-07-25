#include "color.h"
#include <assert.h>

Color::Color(): R(0), G(0), B(0)
{
}

Color::Color( float r, float g, float b): R(r), G(g), B(b)
{
}

Color Color::operator*(const Color& c) const
{
    return Color(R*c.R, G*c.G, B*c.B);
}

Color Color::operator*(const float Factor) const
{
	return Color(Factor*R, Factor*G, Factor*B);
}

Color Color::operator+(const Color& c) const
{
	return Color(R+c.R, G+c.G, B+c.B);
}

Color& Color::operator+=(const Color& c)
{
	R = R + c.R;
	G = G + c.G;
	B = B + c.B;

	return *this;
}