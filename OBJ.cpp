#define MAX_LINE 2000
#include "OBJ.h"

OBJ::OBJ(string filename)
{
	setOBJ(filename);
}

void OBJ::setTexture(string texture)
{
	tx.setTexture(texture);
}

void OBJ::setOBJ(string filename)
{
	char line[MAX_LINE] = "";
	char seps_tag[] = "<>";
	int geometry_done = 0;
	int controllers_done = 0;
	int animations_done = 0;
	int visual_scenes_done = 0;

	ifstream file(filename);
	while (file.getline(line, MAX_LINE)) {
		if (strcmp(line, "") == 0) continue;

		if (geometry_done && controllers_done && animations_done && visual_scenes_done) break;

		char* token = nullptr;
		char* remaining = nullptr;
		token = strtok_s(line, seps_tag, &remaining);
		if (token[0] == '\t') token = strtok_s(NULL, seps_tag, &remaining);

		if (strcmp(token, "library_effects") == 0)
		{
			while (true) {
				file.getline(line, MAX_LINE);
				token = strtok_s(line, seps_tag, &remaining);
				if (token[0] == '\t') token = strtok_s(NULL, seps_tag, &remaining);

				if (strcmp(token, "/lambert") == 0) break;

				else if ((strcmp(token, "emission") == 0)) {
					file.getline(line, MAX_LINE);
					token = strtok_s(line, seps_tag, &remaining);
					if (token[0] == '\t') token = strtok_s(NULL, seps_tag, &remaining);
					token = strtok_s(NULL, seps_tag, &remaining);

					float value[4] = { 0, };
					char seps_value[] = " <";
					char* remaining2 = new char[strlen(token) + 1];
					char* temp = remaining2;
					strcpy_s(remaining2, strlen(token) + 1, token);
					for (int i = 0; i < 4; i++) {
						token = strtok_s(NULL, seps_value, &remaining2);
						value[i] = atof(token);
					}
					mtl.setEmission(value[0],value[1],value[2],value[3]);
					delete[] temp;
					//strtok_s 쓰면서 메모리가 다 사라지는 듯, remaing은 null이지만 
					//원래 주소에는 남아있다!! 아주 까다로웠음
				}
				else if ((strcmp(token, "ambient") == 0)) {
					file.getline(line, MAX_LINE);
					token = strtok_s(line, seps_tag, &remaining);
					if (token[0] == '\t') token = strtok_s(NULL, seps_tag, &remaining);
					token = strtok_s(NULL, seps_tag, &remaining);

					float value[4] = { 0, };
					char seps_value[] = " <";
					char* remaining2 = new char[strlen(token) + 1];
					char* temp = remaining2;
					strcpy_s(remaining2, strlen(token) + 1, token);
					for (int i = 0; i < 4; i++) {
						token = strtok_s(NULL, seps_value, &remaining2);
						value[i] = atof(token);
					}
					mtl.setAmbient(value[0], value[1], value[2], value[3]);
					delete[] temp;
				}
				else if ((strcmp(token, "diffuse") == 0)) {
					file.getline(line, MAX_LINE);
					token = strtok_s(line, seps_tag, &remaining);
					if (token[0] == '\t') token = strtok_s(NULL, seps_tag, &remaining);
					token = strtok_s(NULL, seps_tag, &remaining);

					float value[4] = { 0, };
					char seps_value[] = " <";
					char* remaining2 = new char[strlen(token) + 1];
					char* temp = remaining2;
					strcpy_s(remaining2, strlen(token) + 1, token);
					for (int i = 0; i < 4; i++) {
						token = strtok_s(NULL, seps_value, &remaining2);
						value[i] = atof(token);
					}
					mtl.setDiffuse(value[0], value[1], value[2], value[3]);
					delete[] temp;
				}
			}
		}
		else if (strcmp(token, "library_geometries") == 0)
		{
			int float_array_cnt = 0;
			char seps_att[] = "< >=\"";
			while (true) {
				file.getline(line, MAX_LINE);
				if (strlen(line) <= 1) continue;
				token = strtok_s(line, seps_att, &remaining);
				if (token[0] == '\t') token = strtok_s(NULL, seps_att, &remaining);

				if (strcmp(token, "/library_geometries") == 0) break;

				else if (strcmp(token, "float_array") == 0) {
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);
					float coor[3];
					for (int i = 0; i < count; i++) {
						if (float_array_cnt < 2) {
							file >> coor[i % 3];
							if (i % 3 == 2) {
								if (float_array_cnt == 0) v.push_back(Vector3f(coor[0], coor[1], coor[2]));
								else if (float_array_cnt == 1)vn.push_back(Vector3f(coor[0], coor[1], coor[2]));
							}
						}
						else if (float_array_cnt == 2) {
							file >> coor[i % 2];
							if (i % 2 == 1) {
								v_tx.push_back(Vector2f(coor[0], coor[1]));
							}
						}
					}
					float_array_cnt++;
				}
				else if (strcmp(token, "triangles") == 0) {
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);
					while (strcmp(token, "p") != 0) {
						token = strtok_s(NULL, seps_att, &remaining);
						if (token == NULL) {
							file.getline(line, MAX_LINE);
							token = strtok_s(line, seps_att, &remaining);
						}
					}
					for (int i = 0; i < count; i++) {
						vector<int> temp;
						Face faceInit;
						temp.resize(3);
						file >> temp[0];
						file >> temp[1];
						file >> temp[2];
						faceIdx.push_back(temp);
						faceInit.setV(v[temp[0]], v[temp[1]], v[temp[2]]);
						faceInit.setVN(vn[temp[0]], vn[temp[1]], vn[temp[2]]);
						faceInit.setTX(v_tx[temp[0]], v_tx[temp[1]], v_tx[temp[2]]);
						vFace.push_back(faceInit);
					}
					geometry_done = 1;
				}
			}
		}
		else if (strcmp(token, "library_controllers") == 0) {
			int float_array_cnt = 0;
			char seps_att[] = "< >=\"";
			while (true) {
				file.getline(line, MAX_LINE);
				if (strlen(line) <= 1) continue;
				token = strtok_s(line, seps_att, &remaining);
				if (token[0] == '\t') token = strtok_s(NULL, seps_att, &remaining);

				if (strcmp(token, "/library_controllers") == 0) break;
				else if (strcmp(token, "Name_array") == 0) {
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);
					file.getline(line, MAX_LINE);
					char seps_name[] = " \t";
					token = strtok_s(line, seps_name, &remaining);
					while (token[0] == '\t')token = strtok_s(NULL, seps_name, &remaining);
					jointMap[token] = 0;
					vJoint.push_back(Joint(0, token));
					for (int i = 1; i < count; i++) {
						token = strtok_s(NULL, seps_name, &remaining);
						jointMap[token] = i;
						vJoint.push_back(Joint(i, token));
					}
				}
				else if (strcmp(token, "float_array") == 0) {
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);
					if (float_array_cnt == 0) {
						float arr[16] = { 0, };
						for (int i = 0; i < count; i++) {
							file >> arr[i % 16];
							if (i % 16 == 15) vJoint[i/16].setInverseBindTransform(arr);
						}
						float_array_cnt++;
					}
					else if (float_array_cnt == 1) {
						for (int i = 0; i < count; i++) {
							float wt;
							file >> wt;
							weights.push_back(wt);
						}
					}
				}
				else if (strcmp(token, "vertex_weights") == 0) {
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);
					while (strcmp(token, "vcount") != 0) {
						token = strtok_s(NULL, seps_att, &remaining);
						if (token == NULL) {
							file.getline(line, MAX_LINE);
							token = strtok_s(line, seps_att, &remaining);
						}
					}
					for (int i = 0; i < count; i++) {
						int cnt;
						file >> cnt;
						v_joint_count.push_back(cnt);
					}
					while (strcmp(token, "v") != 0) {
						token = strtok_s(NULL, seps_att, &remaining);
						if (token == NULL) {
							file.getline(line, MAX_LINE);
							if (strlen(line) <= 1)file.getline(line, MAX_LINE);
							token = strtok_s(line, seps_att, &remaining);
						}
					}
					for (int i = 0; i < count; i++) {
						vector<pair<int, int>> jwpairs;
						for (int j = 0; j < v_joint_count[i]; j++) {
							int j_idx, w_idx;
							file >> j_idx;
							file >> w_idx;
							jwpairs.push_back(make_pair(j_idx, w_idx));
						}
						v_joint_weight_pairs.push_back(jwpairs);
					}
					controllers_done = 1;
				}
			}
		}
		else if (strcmp(token, "library_animations") == 0) {

			typedef struct {
				vector<Vector3f> translation;
				vector<float> rotateX;
				vector<float> rotateY;
				vector<float> rotateZ;
				vector<Vector3f> scale;
			}animationData;

			map<string, animationData> data;
			vector<float> timeStamp;

			string name;
			animationData anmData;
			int first_timeStamp = 0;

			char seps_att[] = "< >=\"";
			while (true) {
				file.getline(line, MAX_LINE);
				if (strlen(line) <= 1) continue;
				token = strtok_s(line, seps_att, &remaining);
				if (token[0] == '\t') token = strtok_s(NULL, seps_att, &remaining);

				if (strcmp(token, "/library_animations") == 0) {
					vector<KeyFrame> keyFrames;
					float length;
					length = timeStamp.back();

					for (int i = 0; i < timeStamp.size(); i++) {
						map<string, JointTransform> jt_per_timestamp;
						for (auto it = data.begin(); it != data.end(); it++) {
							Quaternion q(it->second.rotateX[i], it->second.rotateY[i], it->second.rotateZ[i]);
							JointTransform jt(it->second.translation[i],q,it->second.scale[i]);
							jt_per_timestamp.insert(make_pair(it->first, jt));
						}
						keyFrames.push_back(KeyFrame(timeStamp[i], jt_per_timestamp));
					}

					anm.setLength(length);
					anm.setKeyFrames(keyFrames);
					animations_done = 1;
					break;
				}

				if (strcmp(token, "source") == 0) {
					token = strtok_s(NULL, seps_att, &remaining);
					token = strtok_s(NULL, seps_att, &remaining);
				}
				string token_str(token);
				string token_substr;
				string Literal = "TranslationInput";
				if (token_str.size() >= Literal.size()) {
					token_substr = token_str.substr(token_str.size() - Literal.size(), Literal.size());
				}

				if (token_substr == Literal && first_timeStamp == 0) {
					file.getline(line, MAX_LINE);

					//get count
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);

					for (int i = 0; i < count; i++) {
						float time;
						file >> time;
						timeStamp.push_back(time);
					}
					first_timeStamp = 1;
				}

				Literal = "Translation";
				if (token_str.size() >= Literal.size()) {
					token_substr = token_str.substr(token_str.size() - Literal.size(), Literal.size());
				}

				if (token_substr == Literal) {
					name = token_str.substr(0, token_str.size() - Literal.size());
					file.getline(line, MAX_LINE);

					//get count
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);

					for (int i = 0; i < count / 3; i++) {
						float x, y, z;
						file >> x;
						file >> y;
						file >> z;
						anmData.translation.push_back(Vector3f(x, y, z));
					}
				}

				Literal = "RotateX";
				if (token_str.size() >= Literal.size()) {
					token_substr = token_str.substr(token_str.size() - Literal.size(), Literal.size());
				}

				if (token_substr == Literal) {
					file.getline(line, MAX_LINE);

					//get count
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);

					for (int i = 0; i < count; i++) {
						float angle;
						file >> angle;
						anmData.rotateX.push_back(angle);
					}
				}

				Literal = "RotateY";
				if (token_str.size() >= Literal.size()) {
					token_substr = token_str.substr(token_str.size() - Literal.size(), Literal.size());
				}

				if (token_substr == Literal) {
					file.getline(line, MAX_LINE);

					//get count
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);

					for (int i = 0; i < count; i++) {
						float angle;
						file >> angle;
						anmData.rotateY.push_back(angle);
					}
				}

				Literal = "RotateZ";
				if (token_str.size() >= Literal.size()) {
					token_substr = token_str.substr(token_str.size() - Literal.size(), Literal.size());
				}

				if (token_substr == Literal) {
					file.getline(line, MAX_LINE);

					//get count
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);

					for (int i = 0; i < count; i++) {
						float angle;
						file >> angle;
						anmData.rotateZ.push_back(angle);
					}
				}

				Literal = "Scale";
				if (token_str.size() >= Literal.size()) {
					token_substr = token_str.substr(token_str.size() - Literal.size(), Literal.size());
				}

				if (token_substr == Literal) {
					file.getline(line, MAX_LINE);

					//get count
					int count = 0;
					while (!isdigit(token[0])) {
						token = strtok_s(NULL, seps_att, &remaining);
					}
					count = atoi(token);

					for (int i = 0; i < count / 3; i++) {
						float x, y, z;
						file >> x;
						file >> y;
						file >> z;
						anmData.scale.push_back(Vector3f(x, y, z));
					}

					data.insert(make_pair(name, anmData));
					name = "";
					anmData.translation.clear();
					anmData.rotateX.clear();
					anmData.rotateY.clear();
					anmData.rotateZ.clear();
					anmData.scale.clear();
				}
			}
		}
		else if (strcmp(token, "library_visual_scenes") == 0) {
			//node 부터 /node까지가 하나의 hierarchy
			char seps_att[] = "< >=\"";
			while (true) {
				file.getline(line, MAX_LINE);
				if (strlen(line) <= 1) continue;
				token = strtok_s(line, seps_att, &remaining);
				if (token[0] == '\t') token = strtok_s(NULL, seps_att, &remaining);

				if (strcmp(token, "/library_visual_scenes") == 0) {
					visual_scenes_done = 1;
					break;
				}

				else if (strcmp(token, "node") == 0) {
					while (strcmp(token, "id") != 0) token = strtok_s(NULL, seps_att, &remaining);
					token = strtok_s(NULL, seps_att, &remaining);
					if(strcmp(token, "Model") == 0) {
						visual_scenes_done = 1;
						break;
					}
					if (jointMap.find(token) != jointMap.end()) rootJointIdx.push_back(jointMap[token]);
					else {
						jointMap.insert(make_pair(token, jointMap.size()));
						rootJointIdx.push_back(jointMap[token]);
						vJoint.push_back(Joint(jointMap.size()-1,token));
					}
					getInitialTransform(file, token);
					makeJointHierarchy(file, jointMap[token]);
				}
			}
		}
	}
}

void OBJ::getInitialTransform(ifstream& file, string jointName)
{
	char* token = NULL;
	char* remaining = NULL;
	char seps_att[] = "< >=\"";
	char line[MAX_LINE];

	Vector3f tr;
	Quaternion q;
	Vector3f sc;
	float angleX, angleY, angleZ;
	while (true) {
		file.getline(line, MAX_LINE);
		if (strlen(line) <= 1) continue;
		token = strtok_s(line, seps_att, &remaining);
		if (token[0] == '\t') token = strtok_s(NULL, seps_att, &remaining);

		if (strcmp(token, "translate") == 0) {
			while (!isdigit(token[0]) && token[0] != '-') {
				token = strtok_s(NULL, seps_att, &remaining);
			}
			float x, y, z;
			x = atof(token);
			token = strtok_s(NULL, seps_att, &remaining);
			y = atof(token);
			token = strtok_s(NULL, seps_att, &remaining);
			z = atof(token);
			tr.setCoor(x, y, z);
		}
		else if (strcmp(token, "rotate") == 0) {
			while (strcmp(token, "sid") != 0)token = strtok_s(NULL, seps_att, &remaining);
			token = strtok_s(NULL, seps_att, &remaining);
			string sid(token);
			while (!isdigit(token[0]) && token[0] != '-') {
				token = strtok_s(NULL, seps_att, &remaining);
			}
			token = strtok_s(NULL, seps_att, &remaining);
			token = strtok_s(NULL, seps_att, &remaining);
			token = strtok_s(NULL, seps_att, &remaining);
			if (sid== "rotateX") angleX = atof(token);
			else if (sid == "rotateY") angleY = atof(token);
			else if (sid == "rotateZ") {
				angleZ = atof(token);
				q = Quaternion(angleX, angleY, angleZ);
			}
		}
		
		else if (strcmp(token, "scale") == 0) {
			while (!isdigit(token[0]) && token[0] != '-') {
				token = strtok_s(NULL, seps_att, &remaining);
			}
			float x, y, z;
			x = atof(token);
			token = strtok_s(NULL, seps_att, &remaining);
			y = atof(token);
			token = strtok_s(NULL, seps_att, &remaining);
			z = atof(token);
			sc.setCoor(x, y, z);
			initialTransform.insert(make_pair(jointName, JointTransform(tr, q, sc)));
			return;
		}
	}
}

void OBJ::makeJointHierarchy(ifstream& file, int cur_idx)
{
	char* token = NULL;
	char* remaining = NULL;
	char seps_att[] = "< >=\"";
	char line[MAX_LINE];

	while (true) {
		file.getline(line, MAX_LINE);
		if (strlen(line) <= 1) continue;
		token = strtok_s(line, seps_att, &remaining);
		if (token[0] == '\t') token = strtok_s(NULL, seps_att, &remaining);

		if (strcmp(token, "/node") == 0) return;
		else if (strcmp(token, "node") == 0) {
			while (strcmp(token, "id") != 0) token = strtok_s(NULL, seps_att, &remaining);
			token = strtok_s(NULL, seps_att, &remaining);
			if (jointMap.find(token) != jointMap.end()) vJoint[cur_idx].child.push_back(jointMap[token]);
			else {
				jointMap.insert(make_pair(token, jointMap.size()));
				vJoint[cur_idx].child.push_back(jointMap[token]);
				vJoint.push_back(Joint(jointMap.size()-1, token));
			}
			getInitialTransform(file, token);
			makeJointHierarchy(file, jointMap[token]);
		}
	}
}

void OBJ::initialize()
{
	for (int i = 0; i < v.size(); i++) {
		Vector3f new_vertex, new_vertexN;
		for (int j = 0; j < v_joint_count[i]; j++) {
			int joint_idx, weight_idx;
			joint_idx = v_joint_weight_pairs[i][j].first;
			weight_idx = v_joint_weight_pairs[i][j].second;
			new_vertex = new_vertex + weights[weight_idx] * (vJoint[joint_idx].getanimatedTransform() * v[i]);
			new_vertexN = new_vertexN + weights[weight_idx] * (vJoint[joint_idx].getanimatedTransform() * vn[i]);
		}
		v[i] = new_vertex;
		vn[i] = new_vertexN;
	}
	for (int i = 0; i < faceIdx.size(); i++) {
		vFace[i].setV(v[faceIdx[i][0]], v[faceIdx[i][1]], v[faceIdx[i][2]]);
		vFace[i].setVN(vn[faceIdx[i][0]], vn[faceIdx[i][1]], vn[faceIdx[i][2]]);
	}
}

void OBJ::update() 
{
	vector<Vector3f> new_v;
	vector<Vector3f> new_vn;
	for (int i = 0; i < v.size(); i++) {
		Vector3f new_vertex, new_vertexN;
		for (int j = 0; j < v_joint_count[i]; j++) {
			int joint_idx, weight_idx;
			joint_idx = v_joint_weight_pairs[i][j].first;
			weight_idx = v_joint_weight_pairs[i][j].second;
			new_vertex = new_vertex + weights[weight_idx] * (vJoint[joint_idx].getanimatedTransform() * v[i]);
			new_vertexN = new_vertexN + weights[weight_idx] * (vJoint[joint_idx].getanimatedTransform() * vn[i]);
		}
		new_v.push_back(new_vertex);
		new_vn.push_back(new_vertexN);
	}
	for(int i=0;i<faceIdx.size();i++){
		vFace[i].setV(new_v[faceIdx[i][0]], new_v[faceIdx[i][1]], new_v[faceIdx[i][2]]);
		vFace[i].setVN(new_vn[faceIdx[i][0]], new_vn[faceIdx[i][1]], new_vn[faceIdx[i][2]]);
	}
}

void OBJ::draw()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//material
	glPushAttrib(GL_LIGHTING_BIT);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getDiffuse());

	glEnable(GL_TEXTURE_2D);

	//texture 
	for (auto face : vFace) {
		face.draw(tx.getTextureID());
	}
	glPopAttrib();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	
}