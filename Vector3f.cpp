#include "Vector3f.h"

Vector3f::Vector3f(float x, float y, float z)
{
	setCoor(x, y, z);
}

void Vector3f::setCoor(float x, float y, float z)
{
	Coor[0] = x;
	Coor[1] = y;
	Coor[2] = z;
}

float* Vector3f::getCoor()
{
	return Coor;
}

Vector3f operator+(Vector3f v1, Vector3f v2)
{
	float* v1Coor = v1.getCoor();
	float* v2Coor = v2.getCoor();
	return Vector3f(v1Coor[0] + v2Coor[0], v1Coor[1] + v2Coor[1], v1Coor[2] + v2Coor[2]);
}

Vector3f operator*(float c, Vector3f v)
{
	return Vector3f(c * v.getCoor()[0], c * v.getCoor()[1], c * v.getCoor()[2]);
}