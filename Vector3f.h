#pragma once

class Vector3f
{
public:
	Vector3f() { setCoor(0, 0, 0); };
	Vector3f(float x, float y, float z);
	void setCoor(float x, float y, float z);
	float* getCoor();
private:
	float Coor[3];
};

Vector3f operator*(float c, Vector3f v);
Vector3f operator+(Vector3f v1, Vector3f v2);