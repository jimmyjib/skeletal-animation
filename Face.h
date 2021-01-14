#pragma once
#include "Vector2f.h";
#include "Vector3f.h";
#include <GL/glut.h>

class Face
{
public:
	Face() {};
	void setV(Vector3f v1, Vector3f v2, Vector3f v3);
	void setVN(Vector3f v1, Vector3f v2, Vector3f v3);
	void setTX(Vector2f v1, Vector2f v2, Vector2f v3);
	
	void draw(GLuint textureID);
private:
	Vector3f v[3];
	Vector3f vn[3];
	Vector2f tx[3];
	
};

