#pragma once
#include <string>
#include <map>
#include "JointTransform.h"
using namespace std;

class KeyFrame
{
public:
	//copy constructor 호출 문제로 지속적이 오류가 발생되는듯 하다. 
	KeyFrame() {};
	KeyFrame(float timeStamp, map<string, JointTransform> jointKeyFrames) :timeStamp(timeStamp), pose(jointKeyFrames) {};
	void setKeyframe(float time, map<string, JointTransform> jointKeyFrames) {
		timeStamp = time;
		pose = jointKeyFrames;
	}
	float getTimeStamp() { return timeStamp; };
	map<string, JointTransform> getJointTransform();
	void operator=(KeyFrame f) {
		setKeyframe(f.timeStamp, f.pose);
	}
private:
	float timeStamp;
	map<string, JointTransform> pose;
};

