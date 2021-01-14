#pragma once
#include "Matrix4f.h"

class Quaternion
{
public:
	Quaternion() {};
	Quaternion(float phi, float theta, float psi);
	Quaternion(float x, float y, float z, float w) {
		q[0] = x; q[1] = y; q[2] = z; q[3] = w;
	}
	void normalize();
	static Quaternion interpolate(Quaternion start, Quaternion end, float progression) {
		Quaternion result(0, 0, 0, 1);
		float dot = 0;
		for (int i = 0; i < 4; i++) {
			dot += start.q[i] * end.q[i];
		}
		if (dot < 0) dot = -dot;
		for (int i = 0; i < 4; i++) {
			result.q[i] = start.q[i] * (1 - progression) + end.q[i] * progression;
		}
		result.normalize();
		return result;
	}
	Matrix4f toRotationMatrix();
private:
	float q[4];
};

