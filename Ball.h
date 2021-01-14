#pragma once

enum class Color { RED, GREEN, YELLOW, CYAN, WHITE , MAGENTA};

class Ball
{
public:
	Ball();
	Ball(float x, float y, float z, Color color);
	void setColor(Color c);
	void setPos(float x, float y, float z);
	void setPosv(float* p);
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setShininess(float sh);
	void draw() const;

	void setV(float x, float y);
	void setDictIdx(int row, int col);
	void setVisible(bool v);

	float getx();
	float gety();
	float getz();
	float* getPos();
	float getVx();
	float getVy();
	int* getDictIdx();
	bool getVisible();

	Color getColor();
private:
	float pos[3];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess[1];
	float Vx, Vy;
	int dict_idx[2];

	bool visible;
	Color color;
};

