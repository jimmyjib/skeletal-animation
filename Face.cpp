#include "Face.h"

void Face::setV(Vector3f v1, Vector3f v2, Vector3f v3)
{
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
}
void Face::setVN(Vector3f v1, Vector3f v2, Vector3f v3)
{
	vn[0] = v1;
	vn[1] = v2;
	vn[2] = v3;
}
void Face::setTX(Vector2f v1, Vector2f v2, Vector2f v3)
{
	tx[0] = v1;
	tx[1] = v2;
	tx[2] = v3;
}

void Face::draw(GLuint textureID)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_TRIANGLES);
		glNormal3fv(vn[0].getCoor());
		glTexCoord2fv(tx[0].getCoor());
		glVertex3fv(v[0].getCoor());
		glNormal3fv(vn[1].getCoor());
		glTexCoord2fv(tx[1].getCoor());
		glVertex3fv(v[1].getCoor());
		glNormal3fv(vn[2].getCoor());
		glTexCoord2fv(tx[2].getCoor());
		glVertex3fv(v[2].getCoor());
	glEnd();
}