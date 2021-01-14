#include <iostream>
#include "calc.h"
using namespace std;

extern float b_left, b_right, b_top, b_bottom;
extern vector<Ball> balls;
extern vector<Ball> dropping_balls;
extern int num_color;
extern float ball_size;
extern Dict** ball_dict;
extern int ball_row, ball_col;
extern float v;
extern int dropped_balls;

extern bool game_over;

float pointDist(float* a, float* b)
{
	return sqrtf(powf(b[0] - a[0], 2) + powf(b[1] - a[1], 2) + powf(b[2] - a[2], 2));
}

float ballDist(Ball a, Ball b)
{
	return pointDist(a.getPos(), b.getPos());
}

tuple<int, int> getDictIdxForDirec(Ball ball, Direction direc)
{
	int direc_idx[2];
	int* ball_idx = ball.getDictIdx();
	switch (direc) {
	case Direction::RIGHT:
		direc_idx[0] = ball_idx[0];
		direc_idx[1] = ball_idx[1] + 1;
		if (direc_idx[1] < ball_col) return tuple<int, int>(direc_idx[0], direc_idx[1]);
		else return getDictIdxForDirec(ball, Direction::LEFT);
	case Direction::B_RIGHT:
		if (ball_idx[0] % 2 == 0) {
			direc_idx[0] = ball_idx[0] + 1;
			direc_idx[1] = ball_idx[1];
			if (direc_idx[0] < ball_row) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else return getDictIdxForDirec(ball, Direction::T_RIGHT);
		}
		else if (ball_idx[0] % 2 == 1) {
			direc_idx[0] = ball_idx[0] + 1;
			direc_idx[1] = ball_idx[1] + 1;
			if (direc_idx[0] < ball_row && direc_idx[1] < ball_col) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else if(direc_idx[0] >= ball_row) return getDictIdxForDirec(ball, Direction::T_LEFT);
			else if(direc_idx[1] >= ball_col) return getDictIdxForDirec(ball, Direction::LEFT);
		}
		return tuple<int, int>(direc_idx[0], direc_idx[1]);
	case Direction::B_LEFT:
		if (ball_idx[0] % 2 == 0) {
			direc_idx[0] = ball_idx[0] + 1;
			direc_idx[1] = ball_idx[1] - 1;
			if (direc_idx[0] < ball_row && direc_idx[1] >= 0) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else if (direc_idx[0] >= ball_row) return getDictIdxForDirec(ball, Direction::T_RIGHT);
			else if (direc_idx[1] < 0) return getDictIdxForDirec(ball, Direction::RIGHT);
		}
		else if (ball_idx[0] % 2 == 1) {
			direc_idx[0] = ball_idx[0] + 1;
			direc_idx[1] = ball_idx[1];
			if (direc_idx[0] < ball_row) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else return getDictIdxForDirec(ball, Direction::T_LEFT);
		}
	case Direction::LEFT:
		direc_idx[0] = ball_idx[0];
		direc_idx[1] = ball_idx[1] - 1;
		if (direc_idx[1] >= 0) return tuple<int, int>(direc_idx[0], direc_idx[1]);
		else return getDictIdxForDirec(ball, Direction::RIGHT);
	case Direction::T_LEFT:
		if (ball_idx[0] % 2 == 0) {
			direc_idx[0] = ball_idx[0] - 1;
			direc_idx[1] = ball_idx[1] - 1;
			if (direc_idx[0] >= 0 && direc_idx[1] >= 0) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else if (direc_idx[0] < 0) return getDictIdxForDirec(ball, Direction::B_RIGHT);
			else if (direc_idx[1] < 0) return getDictIdxForDirec(ball, Direction::RIGHT);
		}
		else if (ball_idx[0] % 2 == 1) {
			direc_idx[0] = ball_idx[0] - 1;
			direc_idx[1] = ball_idx[1];
			if (direc_idx[0] >= 0) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else return getDictIdxForDirec(ball, Direction::B_LEFT);
		}
	case Direction::T_RIGHT:
		if (ball_idx[0] % 2 == 0) {
			direc_idx[0] = ball_idx[0] - 1;
			direc_idx[1] = ball_idx[1];
			if (direc_idx[0] >= 0) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else return getDictIdxForDirec(ball, Direction::B_RIGHT);
		}
		else if (ball_idx[0] % 2 == 1) {
			direc_idx[0] = ball_idx[0] - 1;
			direc_idx[1] = ball_idx[1] + 1;
			if (direc_idx[0] >= 0 && direc_idx[1] < ball_col) return tuple<int, int>(direc_idx[0], direc_idx[1]);
			else if (direc_idx[0] < 0) return getDictIdxForDirec(ball, Direction::B_RIGHT);
			else if (direc_idx[1] >= ball_col) return getDictIdxForDirec(ball, Direction::LEFT);
		}
	}
}

void fixBallPos(Ball& ball, Ball ref_ball)
{
	int pos_idx = 0;
	float min_dist = 3 * ball_size;
	auto final_dict = getDictIdxForDirec(ref_ball, Direction::RIGHT);
	int final_dict_idx[2];
	int dict_idx[2];
	final_dict_idx[0] = get<0>(final_dict);
	final_dict_idx[1] = get<1>(final_dict);

	float fixed_pos[6][3];
	for (int i = 0; i < 6; i++) {
		auto dict = getDictIdxForDirec(ref_ball, (Direction)i);
		dict_idx[0] = get<0>(dict);
		dict_idx[1] = get<1>(dict);

		fixed_pos[i][0] = ball_dict[dict_idx[0]][dict_idx[1]].pos[0];
		fixed_pos[i][1] = ball_dict[dict_idx[0]][dict_idx[1]].pos[1];
		fixed_pos[i][2] = ball_dict[dict_idx[0]][dict_idx[1]].pos[2];
		
		//cout << "direction " << i << endl;
		//cout << "dict_idx[0]: " << dict_idx[0] << ", dict_idx[1]: " << dict_idx[1] << endl;
		//cout << "x " << ball_dict[dict_idx[0]][dict_idx[1]].pos[0] << " y " << ball_dict[dict_idx[0]][dict_idx[1]].pos[1] << " z " << ball_dict[dict_idx[0]][dict_idx[1]].pos[2] << endl;

		float dist = pointDist(fixed_pos[i], ball.getPos());

		//cout << "dist " << dist << endl;
		if (dist < min_dist) {
			pos_idx = i;
			min_dist = dist;
			final_dict_idx[0] = dict_idx[0];
			final_dict_idx[1] = dict_idx[1];
		}
	}
	//cout << "fixed position x " << fixed_pos[pos_idx][0] << ",y " << fixed_pos[pos_idx][1] << " ,z " << fixed_pos[pos_idx][2] << endl;
	ball.setPosv(fixed_pos[pos_idx]);
	ball.setDictIdx(final_dict_idx[0], final_dict_idx[1]);
	ball_dict[final_dict_idx[0]][final_dict_idx[1]].ball_idx = balls.size();
	ball_dict[final_dict_idx[0]][final_dict_idx[1]].ball_there = true;
}

bool ballStop(Ball& ball)
{
	if (ball.gety() > b_top) {
		int i = 0, flag = 0;
		float x = ball.getx();
		if (x > ball_dict[0][ball_col - 1].pos[0] + ball_size) {
			ball.setPosv(ball_dict[0][ball_col - 1].pos);
			ball.setDictIdx(0, ball_col - 1);
			ball_dict[0][ball_col - 1].ball_idx = balls.size();
			ball_dict[0][ball_col - 1].ball_there = true;
		}
		else {
			for (i = 0; i < ball_col; i++) {
				//cout << i << endl;
				//cout << ball_dict[0][i].pos[0] << endl;
				if (ball_dict[0][i].pos[0] - ball_size < x && x < ball_dict[0][i].pos[0] + ball_size) {
					//cout << "x " << x << endl;
					//cout << "new pos x " << ball_dict[0][i].pos[0] << ", new pos y " << ball_dict[0][i].pos[1] << endl;
					ball.setPosv(ball_dict[0][i].pos);
					ball.setDictIdx(0, i);
					ball_dict[0][i].ball_idx = balls.size();
					ball_dict[0][i].ball_there = true;
				}
			}
		}
		//cout << "end of for" << endl;
		return true;
	}
	else {
		for (Ball other_ball : balls) {
			if (other_ball.getVisible()) {
				float dist = ballDist(other_ball, ball);
				//cout << "before if dist " << dist << endl;
				if (0 < dist && dist < 2 * ball_size) {
					//cout << "after if dist " << dist << endl;
					//cout << "other_ball x " << other_ball.getDictIdx()[0] << " y " << other_ball.getDictIdx()[1] << endl;
					if (ball.gety() < b_bottom + 2 * ball_size) {
						game_over = true;
						return false;
					}
					fixBallPos(ball, other_ball);
					return true;
				}
			}
		}
	}
	return false;
}

bool dropStop(Ball& ball) {
	//cout << ball.gety() << " " << b_bottom << endl;
	if (ball.gety() < b_bottom) return true;
	return false;
}

void connectBall(Ball cur_ball, vector<int>& balls_idx) 
{
	//cout << "connect_size: " << balls_idx.size() << endl;
	for (int i = 0; i < 6; i++) {
		auto dict = getDictIdxForDirec(cur_ball, (Direction)i);
		int dict_idx[2];
		dict_idx[0] = get<0>(dict);
		dict_idx[1] = get<1>(dict);
		Dict& selected_pos = ball_dict[dict_idx[0]][dict_idx[1]];
		if (selected_pos.ball_there && balls[selected_pos.ball_idx].getColor() == cur_ball.getColor() && !selected_pos.visit && cur_ball.getVisible()) {
			//cout << "x: " << dict_idx[0] << " y: " << dict_idx[1] << endl;
			balls_idx.push_back(selected_pos.ball_idx);
			selected_pos.visit = true;
			connectBall(balls[selected_pos.ball_idx], balls_idx);
		}
	}
}

void dropBall(Ball cur_ball, vector<int>& visit)
{
	for (int i = 0; i < 6; i++) {
		auto dict = getDictIdxForDirec(cur_ball, (Direction)i);
		int dict_idx[2];
		dict_idx[0] = get<0>(dict);
		dict_idx[1] = get<1>(dict);
		Dict& selected_pos = ball_dict[dict_idx[0]][dict_idx[1]];
		if (selected_pos.ball_there && !selected_pos.visit) {
			selected_pos.visit = true;
			visit[selected_pos.ball_idx] = 1;
			dropBall(balls[selected_pos.ball_idx], visit);
			selected_pos.visit = false;
		}
	}
}

void mainCalc()
{
	vector<int> balls_idx;
	
	int* last_ball_dict = balls.back().getDictIdx();
	balls_idx.push_back(balls.size() - 1);
	ball_dict[last_ball_dict[0]][last_ball_dict[1]].visit = true;
	connectBall(balls.back(), balls_idx);
	for (auto i : balls_idx) {
		int* dict_idx = balls[i].getDictIdx();
		ball_dict[dict_idx[0]][dict_idx[1]].visit = false;
	}
	ball_dict[last_ball_dict[0]][last_ball_dict[1]].visit = false;

	if (balls_idx.size() >= 3) {
		for (int i : balls_idx) {
			int* temp_dict = balls[i].getDictIdx();
			ball_dict[temp_dict[0]][temp_dict[1]].ball_there = false;
			balls[i].setVisible(false);
			dropped_balls++;
			dropping_balls.push_back(balls[i]);
			dropping_balls.back().setV(randomV(v / 2), v / 10);
		}
	}
	
	vector<int> vt;
	vt.resize(balls.size());
	for (int i = 0; i < vt.size(); i++)vt[i] = 0;
	for (int i = 0; i < ball_col; i++) {
		if (ball_dict[0][i].ball_there) {
			vt[ball_dict[0][i].ball_idx] = 1;
			ball_dict[0][i].visit = true;
			dropBall(balls[ball_dict[0][i].ball_idx], vt);
			ball_dict[0][i].visit = false;
		}
	}
	for (int i = 0; i < balls.size(); i++) {
		int* temp_dict = balls[i].getDictIdx();
		if (vt[i] == 0 && ball_dict[temp_dict[0]][temp_dict[1]].ball_there && balls[i].getVisible()) {
			ball_dict[temp_dict[0]][temp_dict[1]].ball_there = false;
			balls[i].setVisible(false);
			dropped_balls++;
			dropping_balls.push_back(balls[i]);
			dropping_balls.back().setV(randomV(v / 2), v / 10);
		}
	}
}
