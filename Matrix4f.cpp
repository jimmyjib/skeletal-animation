#include "Matrix4f.h"

void Matrix4f::setMatrix(float* arr)
{
	for (int i = 0; i < 4; i++) {
		value[0][i] = arr[i];
		value[1][i] = arr[i + 4];
		value[2][i] = arr[i + 8];
		value[3][i] = arr[i + 12];
	}
}

float rec_det(int size, vector<vector<float>> mat) {
	if (size == 2) {
		return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
	}
	else if (size == 1) return mat[0][0];
	float sum = 0;
	vector<vector<float>> submat;
	for (int i = 0; i < mat.size(); i++) {
		for (int j = 1; j < mat.size(); j++) {
			vector<float> row;
			for (int k = 0; k < mat.size(); k++) {
				if (k != i) row.push_back(mat[j][k]);
			}
			submat.push_back(row);
		}
		sum += powf(-1, i) * mat[0][i] * rec_det(size - 1, submat);
		submat.clear();
	}
	return sum;
}

float Matrix4f::det()
{
	vector<vector<float>>mat;
	for (int i = 0; i < 4; i++) {
		vector<float> row;
		for (int j = 0; j < 4; j++) {
			row.push_back(value[i][j]);
		}
		mat.push_back(row);
	}
	float determinant = rec_det(4, mat);
	return determinant;
}

vector<vector<float>> Matrix4f::makeSubMat(int r, int c)
{
	vector<vector<float>>mat;
	for (int i = 0; i < 4; i++) {
		vector<float> row;
		if (i != r) {
			for (int j = 0; j < 4; j++) {
				if (j != c)row.push_back(value[i][j]);
			}
			mat.push_back(row);
		}
	}
	return mat;
}

Matrix4f Matrix4f::inverse()
{
	float determinant = det();
	float arr[16] = { 0, };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			arr[4 * i + j] = powf(-1, i + j) * rec_det(3, makeSubMat(j, i));
		}
	}
	return Matrix4f(arr);
}

Vector3f operator*(Matrix4f mat4, Vector3f v) {
	float* vCoor = v.getCoor();
	float v4[4];
	v4[0] = vCoor[0]; v4[1] = vCoor[1]; v4[2] = vCoor[2]; v4[3] = 1;
	float ans[3];
	for (int i = 0; i < 3; i++) {
		ans[i] = mat4.get()[i][0] * v4[0] + mat4.get()[i][1] * v4[1] + mat4.get()[i][2] * v4[2] + mat4.get()[i][3] * v4[3];
	}
	return Vector3f(ans[0], ans[1], ans[2]);
}

Matrix4f operator*(Matrix4f mat1, Matrix4f mat2) 
{
	Matrix4f mat;
	float arr[16] = { 0, };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				arr[4 * i + j] += mat1.get()[i][k] * mat2.get()[k][j];
			}
		}
	}
	mat.setMatrix(arr);
	return mat;
}

void Matrix4f::loadIdentity()
{
	float arr[16] = { 0, };
	setMatrix(arr);
	for (int i = 0; i < 4; i++) {
		value[i][i] = 1;
	}
}

void Matrix4f::translate(Vector3f tr)
{
	loadIdentity();
	for (int i = 0; i < 3; i++) {
		value[i][3] = tr.getCoor()[i];
	}
}

void Matrix4f::scale(Vector3f sc)
{
	loadIdentity();
	for (int i = 0; i < 3; i++) {
		value[i][i] = sc.getCoor()[i];
	}
}

void Matrix4f::rotate(float angle, Axis ax)
{
	loadIdentity();
	switch (ax)
	{
	case Axis::x:
		value[1][1] = cos(angle * M_PI / 180);
		value[1][2] = -sin(angle * M_PI / 180);
		value[2][1] = sin(angle * M_PI / 180);
		value[2][2] = cos(angle * M_PI / 180);
		break;
	case Axis::y:
		value[0][0] = cos(angle * M_PI / 180);
		value[0][2] = -sin(angle * M_PI / 180);
		value[2][0] = sin(angle * M_PI / 180);
		value[2][2] = cos(angle * M_PI / 180);
		break;
	case Axis::z:
		value[0][0] = cos(angle * M_PI / 180);
		value[0][1] = -sin(angle * M_PI / 180);
		value[1][0] = sin(angle * M_PI / 180);
		value[1][1] = cos(angle * M_PI / 180);
		break;
	default:
		break;
	}
}