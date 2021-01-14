#pragma once
#include "Quaternion.h"
#include "Matrix4f.h"

class JointTransform
{
public:
	JointTransform() {};
	JointTransform(Vector3f position, Quaternion rotation, Vector3f scale) :
		position(position), rotation(rotation), scale(scale) {};
	static Vector3f vector3fInterpolate(Vector3f start, Vector3f end, float progression) {
		float Coor[3];
		for (int i = 0; i < 3; i++) {
			Coor[i] = start.getCoor()[i] + (end.getCoor()[i] - start.getCoor()[i]) * progression;
		}
		return Vector3f(Coor[0], Coor[1], Coor[2]);
	}
	Matrix4f getLocalTransformMatrix();
	static JointTransform interpolate(JointTransform frameA, JointTransform frameB, float progression) {
		Vector3f pos = vector3fInterpolate(frameA.position, frameB.position, progression);
		Quaternion rot = Quaternion::interpolate(frameA.rotation, frameB.rotation, progression);
		Vector3f sc = vector3fInterpolate(frameA.scale, frameB.scale, progression);
		return JointTransform(pos, rot, sc);
	}
private:
	
	Vector3f position;
	Quaternion rotation;
	Vector3f scale;
};

