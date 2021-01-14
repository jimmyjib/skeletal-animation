#pragma once
#include <vector>
#include <fstream>
#include <string.h>
#include <map>
#include "Material.h"
#include "Texture.h"
#include "Face.h"
#include "Joint.h"
#include "Animation.h"

using namespace std;

class OBJ
{
public:
	OBJ() {};
	OBJ(string filename);
	void setTexture(string texture);
	void setOBJ(string filename);
	void update();
	void initialize();
	void draw();
	
	friend class Animator;

private:
	void makeJointHierarchy(ifstream& file, int cur_idx);
	void getInitialTransform(ifstream& file, string jointName);
	vector<Face> vFace;
	vector<vector<int>> faceIdx;
	vector<int> rootJointIdx;
	vector<Joint> vJoint;
	vector<int> v_joint_count;
	vector<float> weights;
	vector<vector<pair<int, int>>> v_joint_weight_pairs;
	vector<Vector3f> v;
	vector<Vector3f> vn;
	vector<Vector2f> v_tx;
	map<string, JointTransform> initialTransform;
	Texture tx;
	Material mtl;
	map<string, int> jointMap;
	Animation anm;

};

