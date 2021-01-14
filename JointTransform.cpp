#include "JointTransform.h"

Matrix4f JointTransform::getLocalTransformMatrix()
{
	Matrix4f mt_tr, mt_sc, mt;
	mt_tr.translate(position);
	mt_sc.scale(scale);
	mt = mt_sc * mt_tr * rotation.toRotationMatrix();
	//mt = mt_sc * mt_tr;
	//mt = mt_sc * rotation.toRotationMatrix() * mt_tr;
	//mt = rotation.toRotationMatrix();
	return mt;
}