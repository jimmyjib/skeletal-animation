#include "Ball.h"
#include <GL/glut.h>

extern float ball_size;

Ball::Ball()
{
	setPos(0, 0, 0);
	setColor(Color::WHITE);
	setV(0, 0);
	visible = true;
}

Ball::Ball(float x, float y, float z, Color color)
{
	setPos(x, y, z);
	setColor(color);
	setV(0, 0);
	visible = true;
}

void Ball::setColor(Color c)
{
	color = c;
	switch (c) {
	case Color::RED:
		setAmbient(0.1, 0, 0, 1.0);
		setDiffuse(0.5, 0, 0, 1.0);
		setSpecular(0.7, 0.6, 0.6, 1.0);
		setShininess(32);
		break;
	case Color::CYAN:
		setAmbient(0, 0.1, 0.06, 1.0);
		setDiffuse(0, 0.50980392, 0.50980392, 1.0);
		setSpecular(0.50196078, 0.50196078, 0.50196078, 1.0);
		setShininess(32);
		break;
	case Color::GREEN:
		setAmbient(0, 0.1, 0, 1.0);
		setDiffuse(0.1, 0.35, 0.1, 1.0);
		setSpecular(0.45, 0.55, 0.45, 1.0);
		setShininess(32);
		break;
	case Color::WHITE:
		setAmbient(0.1, 0.1, 0.1, 1.0);
		setDiffuse(0.55, 0.55, 0.55, 1.0);
		setSpecular(0.7, 0.7, 0.7, 1.0);
		setShininess(32);
		break;
	case Color::YELLOW:
		setAmbient(0.1, 0.1, 0, 1.0);
		setDiffuse(0.5, 0.5, 0, 1.0);
		setSpecular(0.6, 0.6, 0.5, 1.0);
		setShininess(32);
		break;
	case Color::MAGENTA:
		setAmbient(0.1, 0, 0.1, 1.0);
		setDiffuse(0.5, 0, 0.5, 1.0);
		setSpecular(0.7, 0.6, 0.6, 1.0);
		setShininess(32);
		break;
	}
}

void Ball::setPos(float x, float y, float z)
{
	pos[0] = x; pos[1] = y; pos[2] = -z;
}

void Ball::setPosv(float* p)
{
	pos[0] = p[0]; pos[1] = p[1]; pos[2] = p[2];
}

void Ball::setAmbient(float r, float g, float b, float a)
{
	ambient[0] = r; ambient[1] = g; ambient[2] = b; ambient[3] = a;
}

void Ball::setDiffuse(float r, float g, float b, float a)
{
	diffuse[0] = r; diffuse[1] = g; diffuse[2] = b; diffuse[3] = a;
}

void Ball::setSpecular(float r, float g, float b, float a)
{
	specular[0] = r; specular[1] = g; specular[2] = b; specular[3] = a;
}

void Ball::setShininess(float sh)
{
	shininess[0] = sh;
}

void Ball::draw() const
{
	glPushMatrix();

	glTranslatef(pos[0], pos[1], pos[2]);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	float emission[4] = { 0.3,0.3,0.3,0 };
	glPushAttrib(GL_LIGHTING_BIT);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	
	glutSolidSphere(ball_size, 30, 30);
	glPopAttrib();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glPopMatrix();
}

void Ball::setV(float x, float y)
{
	Vx = x; Vy = y;
}

void Ball::setDictIdx(int row, int col)
{
	dict_idx[0] = row; dict_idx[1] = col;
}

void Ball::setVisible(bool v)
{
	visible = v;
}

float Ball::getx()
{
	return pos[0];
}

float Ball::gety()
{
	return pos[1];
}

float Ball::getz()
{
	return pos[2];
}

float* Ball::getPos()
{
	return pos;
}

float Ball::getVx()
{
	return Vx;
}

float Ball::getVy()
{
	return Vy;
}

int* Ball::getDictIdx()
{
	return dict_idx;
}

Color Ball::getColor()
{
	return color;
}

bool Ball::getVisible()
{
	return visible;
}

