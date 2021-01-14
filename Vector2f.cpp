#include "Vector2f.h"

Vector2f::Vector2f(float s, float t)
{
	setCoor(s, t);
}

void Vector2f::setCoor(float s, float t)
{
	Coor[0] = s;
	Coor[1] = t;
}

float* Vector2f::getCoor()
{
	return Coor;
}