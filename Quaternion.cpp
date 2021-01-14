#include "Quaternion.h"

//각도를 일단 euler angle(intrinsic으로 가정)->rotation matrix를 계산하는 식은 정확
//but animation 상의 각도가 euler rotation matrix가 아님
//y -> -y?? --> 역행렬이 transpose 돼서 나옴
//전부 -& transpos-->유레카!!
Quaternion::Quaternion(float phi, float theta, float psi)
{
	theta = -theta;
	phi = -phi;
	psi = -psi;
	const float phi_2_rad = phi / 2 * M_PI / 180;
	const float theta_2_rad = theta / 2 * M_PI / 180;
	const float psi_2_rad = psi / 2 * M_PI / 180;
	q[0] = sin(phi_2_rad) * cos(theta_2_rad) * cos(psi_2_rad) - cos(phi_2_rad) * sin(theta_2_rad) * sin(psi_2_rad);
	q[1] = cos(phi_2_rad) * sin(theta_2_rad) * cos(psi_2_rad) + sin(phi_2_rad) * cos(theta_2_rad) * sin(psi_2_rad);
	q[2] = cos(phi_2_rad) * cos(theta_2_rad) * sin(psi_2_rad) - sin(phi_2_rad) * sin(theta_2_rad) * cos(psi_2_rad);
	q[3] = cos(phi_2_rad) * cos(theta_2_rad) * cos(psi_2_rad) + sin(phi_2_rad) * sin(theta_2_rad) * sin(psi_2_rad);
	/*
	이건 진짜 아닌듯
	q[0] = cos((phi - psi) / 2 * M_PI / 180) * sin(theta / 2 * M_PI / 180);
	q[1] = sin((phi - psi) / 2 * M_PI / 180) * sin(theta / 2 * M_PI / 180);
	q[2] = sin((phi + psi) / 2 * M_PI / 180) * sin(theta / 2 * M_PI / 180);
	q[3] = cos((phi + psi) / 2 * M_PI / 180) * sin(theta / 2 * M_PI / 180);
	*/
}

Matrix4f Quaternion::toRotationMatrix()
{
	float arr[16];
	arr[0] = 1 - 2 * powf(q[1], 2) - 2 * powf(q[2], 2);
	arr[4] = 2 * (q[0] * q[1] - q[2] * q[3]);
	arr[8] = 2 * (q[0] * q[2] + q[1] * q[3]);
	arr[12] = 0;

	arr[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
	arr[5] = 1 - 2 * powf(q[0], 2) - 2 * powf(q[2], 2);
	arr[9] = 2 * (q[1] * q[2] - q[0] * q[3]);
	arr[13] = 0;

	arr[2] = 2 * (q[0] * q[2] - q[1] * q[3]);
	arr[6] = 2 * (q[0] * q[3] + q[1] * q[2]);
	arr[10] = 1 - 2 * powf(q[0], 2) - 2 * powf(q[1], 2);
	arr[14] = 0;

	arr[3] = 0;
	arr[7] = 0;
	arr[11] = 0;
	arr[15] = 1;

	return Matrix4f(arr);
}

void Quaternion::normalize() 
{
	float size = sqrtf(powf(q[0], 2) + powf(q[1], 2) + powf(q[2], 2) + powf(q[3], 2));
	for (int i = 0; i < 4; i++) {
		q[i] /= size;
	}
}