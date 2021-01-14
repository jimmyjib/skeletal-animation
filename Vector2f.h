#pragma once

class Vector2f
{
public:
	Vector2f() {};
	Vector2f(float s, float t);
	void setCoor(float s, float t);
	float* getCoor();
private:
	float Coor[2];
};

