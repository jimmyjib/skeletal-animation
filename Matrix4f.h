#pragma once
#define _USE_MATH_DEFINES
#include "Vector3f.h"
#include <math.h>
#include <vector>
using namespace std;

typedef float row[4];

enum class Axis { x, y, z };

class Matrix4f
{
public:
	Matrix4f() {};
	Matrix4f(float* arr) { setMatrix(arr); };
	void setMatrix(float* arr);
	row* get() { return value; };

	void loadIdentity();
	void translate(Vector3f tr);
	void scale(Vector3f sc);
	void rotate(float angle, Axis ax);
	Matrix4f inverse();
	float det();

private:
	vector<vector<float>> makeSubMat(int row, int col);
	float value[4][4];
};

Vector3f operator*(Matrix4f mat4, Vector3f v);
Matrix4f operator*(Matrix4f mat1, Matrix4f mat2);
