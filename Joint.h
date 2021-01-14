#pragma once
#include "Matrix4f.h"
#include <vector>
#include <string>
#include <GL/glut.h>
using namespace std;

class Joint
{
public:
	Joint(int index, string name) :idx(index), name(name) {}

	void setInverseBindTransform(float* arr) { inverseBindTransform.setMatrix(arr); }
	void setInverseBindTransform(Matrix4f mat4) { inverseBindTransform = mat4; }
	void setAnimatedTransform(Matrix4f mat4) { animatedTransform = mat4; }
	void addchild(int idx) { child.push_back(idx); }
	Matrix4f getInverseBindTransform() { return inverseBindTransform; };
	Matrix4f getanimatedTransform() { return animatedTransform; };

	int idx;
	string name;
	vector<int> child;
private:
	Matrix4f inverseBindTransform;
	Matrix4f animatedTransform;
};

