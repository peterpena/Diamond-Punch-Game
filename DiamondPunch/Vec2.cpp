// Vec2.cpp
// By Frank Luna
// August 24, 2004.

#include "Vec2.h"
#include <cmath>

Vec2::Vec2()
: x(0.0f), y(0.0f)
{
}

Vec2::Vec2(float s, float t)
: x(s), y(t)
{
}

Vec2::Vec2(float d[2])
: x(d[0]), y(d[1])
{
}

Vec2::Vec2(const POINT& p)
: x((float)p.x), y((float)p.y)
{
}

Vec2 Vec2::operator+(const Vec2& rhs)const
{
	Vec2 result;
	result.x = x + rhs.x;
	result.y = y + rhs.y;

	return result;
}

Vec2 Vec2::operator-(const Vec2& rhs)const
{
	Vec2 result;
	result.x = x - rhs.x;
	result.y = y - rhs.y;

	return result;
}

Vec2 Vec2::operator-()
{
	Vec2 result;
	result.x = -x;
	result.y = -y;

	return result;
}

Vec2::operator POINT()
{
	POINT p = {(int)x, (int)y};
	return p;
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(float s)
{
	x *= s;
	y *= s;
}

void Vec2::operator/=(float s)
{
	// Assumes s != 0.
	s /= s;
	y /= s;
}

bool Vec2::operator==(const Vec2& rhs)
{
    if((x == rhs.x) && (y == rhs.y))
        return true;

    else
        return false;
}

float Vec2::length()
{
	return sqrtf(x*x + y*y);
}

Vec2& Vec2::normalize()
{
	// Assumes len != 0.
	float len = length();

	x /= len;
	y /= len;

	return *this;
}

float Vec2::dot(const Vec2& rhs)
{
	return x*rhs.x + y*rhs.y;
}

Vec2& Vec2::reflect(const Vec2& normal)
{
	Vec2 I = *this;

	return *this = 2.0f * -I.dot(normal) * normal + I;
}

Vec2 operator*(const Vec2& v, float s)
{
	Vec2 result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

Vec2 operator*(float s, const Vec2& v)
{
	Vec2 result;

	result.x = v.x * s;
	result.y = v.y * s;

	return result;
}

Vec2 operator/(const Vec2& v, float s)
{
	Vec2 result;

	// Assumes s != 0.
	result.x = v.x / s;
	result.y = v.y / s;

	return result;
}

Vec2 operator/(float s, const Vec2& v)
{
	Vec2 result;

	// Assumes s != 0.
	result.x = v.x / s;
	result.y = v.y / s;

	return result;
}
