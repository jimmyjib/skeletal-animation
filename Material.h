#pragma once
class Material
{
public:
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setEmission(float r, float g, float b, float a);
	void setShininess(float sh);
	float* getAmbient() { return ambient; };
	float* getDiffuse() { return diffuse; };
	float* getSpecular() { return specular; };
	float* getEmission() { return emission; };
	float* getShininess() { return shininess; };
private:
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shininess[1];
};

