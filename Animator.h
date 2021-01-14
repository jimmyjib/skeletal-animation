#pragma once
#include "OBJ.h"
class Animator
{
public:
	Animator() { 
		animationTime = 0; 
		initialization = 0; 
		loopEndPassed = false; 
		transition = false; 
		swing = false; 
		swingDone = false;
	}
	void setAnimation(OBJ& obj);

	//called every frame
	map<string, Matrix4f>& interpolatePoses(KeyFrame& f1, KeyFrame& f2, float progression);
	pair<KeyFrame*, KeyFrame*>& getPreviousAndNextFrames();
	float calculateProgression(KeyFrame& f1, KeyFrame& f2);
	
	map<string, Matrix4f>& calculatePose();
	void applyPoseToJoints(map<string, Matrix4f>& currentPose, Joint& currentJoint, Matrix4f& parentMatrix);
	void initialize();
	void manageTime();
	void update();

	void setAnimationTime(float time);
	float getAnimationTime();
	OBJ obj;

	bool swing;
	void startSwing() { swing = true; };

private:
	bool loopEndPassed;
	bool transition;
	bool swingDone;
	int initialization;
	Animation currentAnimation;
	float animationTime;
};

