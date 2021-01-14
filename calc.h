#pragma once
#define _USE_MATH_DEFINES
#include "Ball.h"
#include "random.h"
#include "others.h"
#include <math.h>
#include <tuple>
#include <vector>
using namespace std;

enum class Direction { RIGHT, B_RIGHT, B_LEFT, LEFT, T_LEFT, T_RIGHT };

bool ballStop(Ball& ball);

bool dropStop(Ball& ball);

float pointDist(float* a, float* b);

float ballDist(Ball a, Ball b);

void connectBall(Ball cur_ball, vector<int>& balls_idx);

void dropBall(Ball cur_ball, vector<int>& visit);

void mainCalc();

void fixBallPos(Ball& ball, Ball ref_ball);

tuple<int,int> getDictIdxForDirec(Ball ball, Direction direc);