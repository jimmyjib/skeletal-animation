#include "Animator.h"

void Animator::setAnimation(OBJ& object)
{
	obj = object;
	animationTime = 0;
	currentAnimation = object.anm;
}

map<string, Matrix4f>& Animator::interpolatePoses(KeyFrame& f1, KeyFrame& f2, float progression)
{
	map<string, Matrix4f>* currentPose = new map<string, Matrix4f>;
	for (auto it = obj.jointMap.begin(); it != obj.jointMap.end(); it++) {
		JointTransform previous, next, current;
		string jointName = it->first;
		previous = f1.getJointTransform()[jointName];
		next = f2.getJointTransform()[jointName];
		current = JointTransform::interpolate(previous, next, progression);
		(*currentPose).insert(make_pair(jointName, current.getLocalTransformMatrix()));
	}
	return (*currentPose);
}

pair<KeyFrame*, KeyFrame*>& Animator::getPreviousAndNextFrames()
{
	vector<KeyFrame>& allFrames = currentAnimation.getKeyFrames();
	int prev_idx = 0, next_idx = 0;
	for (int i = 1; i < allFrames.size(); i++) {
		next_idx = i;
		if (allFrames[i].getTimeStamp() > animationTime) break;
		prev_idx = i;
	}
	pair<KeyFrame*, KeyFrame*>f = make_pair(&allFrames[prev_idx], &allFrames[next_idx]);
	return f;
}

float Animator::calculateProgression(KeyFrame& f1, KeyFrame& f2)
{
	return (animationTime - f1.getTimeStamp()) / (f2.getTimeStamp() - f1.getTimeStamp());
}

map<string, Matrix4f>& Animator::calculatePose()
{
	pair<KeyFrame*, KeyFrame*>& frames = getPreviousAndNextFrames();
	KeyFrame& f1 = *frames.first;
	KeyFrame& f2 = *frames.second;
	float progression = calculateProgression(f1, f2);
	return interpolatePoses(f1, f2, progression); 
}
void Animator::applyPoseToJoints(map<string, Matrix4f>& currentPose, Joint& currentJoint, Matrix4f& parentMatrix)
{
	Matrix4f& currentLocalTransform = currentPose[currentJoint.name];
	//parent*child 순 자전 후 공전 느낌
	Matrix4f currentTransform = parentMatrix * currentLocalTransform;
	//Matrix4f currentTransform = currentLocalTransform * parentMatrix;
	for (auto childIdx : currentJoint.child) {
		applyPoseToJoints(currentPose, obj.vJoint[childIdx], currentTransform);
	}
	/*
	if (initialization == 1) currentTransform = currentTransform * currentJoint.getInverseBindTransform();
	else {
		currentJoint.setInverseBindTransform(currentTransform.inverse());
		currentTransform = currentTransform * currentTransform.inverse();
	}
	*/
	currentTransform = currentTransform * currentJoint.getInverseBindTransform();
	currentJoint.setAnimatedTransform(currentTransform);
}

void Animator::initialize()
{
	map<string, Matrix4f> initialpose;
	for (auto it = obj.initialTransform.begin(); it != obj.initialTransform.end(); it++) {
		initialpose.insert(make_pair(it->first, it->second.getLocalTransformMatrix()));
	}
	for (auto rootIdx : obj.rootJointIdx) {
		Matrix4f Identity;
		Identity.loadIdentity();
		//Matrix4f translate;
		//translate.translate(Vector3f(1000, 2000, 3000));
		applyPoseToJoints(initialpose, obj.vJoint[rootIdx], Identity);
	}
	initialization = 1;
	obj.initialize();
}

void Animator::manageTime()
{
	const float loopStart = 3.35;
	const float loopEnd = 3.60;
	const float swingEnd = 4.4;
	const float scale = 0.2;
	if (!swing) {
		if (animationTime >= loopEnd && !loopEndPassed) {
			loopEndPassed = true;
		}
		if (animationTime < loopStart && loopEndPassed) {
			loopEndPassed = false;
			transition = true;
		}
		if (animationTime < loopStart) {
			if (!transition) animationTime += 0.04 * scale;
			else animationTime += 0.025 * scale;
		}
		else if (animationTime >= loopStart && animationTime < loopEnd && !loopEndPassed) {
			transition = false;
			animationTime += 0.025 * scale;
		}
		else if (loopEndPassed) {
			animationTime -= 0.025 * scale;
		}
	}
	else if (swing) {
		if (animationTime < swingEnd && !swingDone) {
			if (animationTime > swingEnd - 0.1)animationTime += 0.02 * scale;
			else animationTime += 0.08 * scale;
			if (animationTime >= swingEnd) swingDone = true;
		}
		else if (swingDone) {
			if (animationTime > swingEnd - 0.1) animationTime -= 0.04 * scale;
			else animationTime -= 0.08 * scale;
			if (animationTime <= loopEnd) {
				swing = false;
				swingDone = false;
			}
		}
	}
}

void Animator::setAnimationTime(float time) 
{
	animationTime = time;
}

float Animator::getAnimationTime() 
{
	return animationTime;
}

void Animator::update()
{
	map<string, Matrix4f>& currentPose = calculatePose();
	//animationTime += 0.03;
	manageTime();
	for (auto rootIdx : obj.rootJointIdx) {
		Matrix4f Identity;
		Identity.loadIdentity();

		applyPoseToJoints(currentPose, obj.vJoint[rootIdx], Identity);
	}
	delete &currentPose;
	obj.update();
}