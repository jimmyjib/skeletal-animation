#pragma once
#include <iostream>
#include <FreeImage.h>
#include <GL/glut.h>
using namespace std;

class Texture
{
public:
	void setTexture(string filename);
	void setTextureID();
	GLuint getTextureID() { return textureID; };
private:
	int imageWidth;
	int imageHeight;
	GLubyte* textureData;
	GLuint textureID;
};

