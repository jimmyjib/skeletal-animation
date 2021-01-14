#pragma once
#include <vector>
#include "KeyFrame.h"

using namespace std;

class Animation
{
public:
	Animation() {};
	void setLength(float length) { total_length = length; };
	void setKeyFrames(vector<KeyFrame> keyFrames) { vKeyFrames = keyFrames; };
	float getLength() { return total_length; };
	vector<KeyFrame>& getKeyFrames();
private:
	//seconds;
	float total_length;
	vector<KeyFrame> vKeyFrames;
};

