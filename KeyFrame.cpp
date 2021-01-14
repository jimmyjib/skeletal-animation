#include "KeyFrame.h"

map<string, JointTransform> KeyFrame::getJointTransform()
{ 
	map<string, JointTransform> temp;
	temp = pose;
	return temp; 
}