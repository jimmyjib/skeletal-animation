#include "random.h"
using namespace std;

int randomColor(int range)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, range-1);

	return dis(gen);
}

float randomV(float v)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(-5, 5);

	return v * dis(gen) / 10;
}