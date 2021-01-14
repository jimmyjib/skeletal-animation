//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
#include "OBJ.h"
#include "Animator.h"
#include "calc.h"

#define WIDTH 1280 
#define HEIGHT 720 
#define DEPTH 1500

int num_color = 4;
float angle = 90;
double ang_rad = angle * M_PI / 180;
float time_moved = 0;
float ball_size = 15;
float v = 10;
float b_left = 50, b_right = 250, b_top = 170, b_bottom = -100;
float lb_left = b_left - 100 - ball_size, lb_height = 30;
int dropped_balls = 0;
const float autoLaunchLimit = 10;
float time_left = autoLaunchLimit;

//속도가 너무 빠르면 collision detection에 문제가 생김
vector<Ball> balls;
vector<Ball> dropping_balls;
Vector2f leaderboard_pos[5];

int ball_row, ball_col;
Dict** ball_dict;

Ball ball_ready, ball_next;

bool pause = false;
OBJ veigar("taunt.dae");
Animator animator;
Texture floor_tx, wall_tx, board_tx;
Texture arrow_tx_1, arrow_tx_2, case_tx;
Texture leaderboard_tx, restart_tx, close_tx, game_over_tx;
Texture emblem_tx[5];

vector<int> score(4, 0);
int cur_score = 0;

bool game_over = false;

void init()
{
	veigar.setTexture("./textures/veigar_skin08_tx_cm.png");
	floor_tx.setTexture("./textures/floor.png");
	wall_tx.setTexture("./textures/wall.png");
	board_tx.setTexture("./textures/board.jpg");
	arrow_tx_1.setTexture("./textures/arrow1.png");
	arrow_tx_2.setTexture("./textures/arrow2.png");
	case_tx.setTexture("./textures/case.png");
	leaderboard_tx.setTexture("./textures/leaderboard_case.png");
	game_over_tx.setTexture("./textures/gameover.jpg");
	restart_tx.setTexture("./textures/restart.png");
	close_tx.setTexture("./textures/shutdown.png");
	animator.setAnimation(veigar);
	//initialize를 하면 inverse와 곱해져 항등행렬이 나와야 하지 않나?
	//animator.initialize();
	for (int i = 0; i < 5; i++) {
		string emblem = "./textures/mastery_icon_";
		emblem = emblem + to_string(i) + ".png";
		leaderboard_pos[i].setCoor(lb_left - 15, b_top - 15 - (lb_height + 10) * i);
		emblem_tx[i].setTexture(emblem);
	}
}

void scoreInit()
{
	//cout << "scoreInit" << endl;
	cur_score = 0;
	ifstream score_in("score.txt");
	for (int i = 0; i < 4; i++) {
		int sc;
		score_in >> sc;
		if (score_in.eof()) return;
		//cout << sc << endl;
		score[i] = sc;
	}
	score_in.close();
}

void ballDictInit()
{
	ball_row = (int)((b_top - b_bottom) / (sqrtf(3) * ball_size)) + 1;
	ball_col = (int)((b_right - b_left) / (2 * ball_size)) + 1;
	ball_dict = new Dict * [ball_row];
	for (int i = 0; i < ball_row; i++) {
		ball_dict[i] = new Dict[ball_col];
	}

	for (int i = 0; i < ball_col; i++) {
		ball_dict[0][i].pos[0] = b_left + 2 * i * ball_size;
		ball_dict[0][i].pos[1] = b_top;
		ball_dict[0][i].pos[2] = 0;
		ball_dict[0][i].ball_there = false;
		ball_dict[0][i].visit = false;
	}

	for (int i = 1; i < ball_row; i++) {
		for (int j = 0; j < ball_col; j++) {
			if (i % 2 == 1) {
				ball_dict[i][j].pos[0] = ball_dict[i - 1][j].pos[0] + ball_size;
				ball_dict[i][j].pos[1] = ball_dict[i - 1][j].pos[1] - ball_size * sqrt(3);
				ball_dict[i][j].pos[2] = 0;
			}
			else if (i % 2 == 0) {
				ball_dict[i][j].pos[0] = ball_dict[i - 1][j].pos[0] - ball_size;
				ball_dict[i][j].pos[1] = ball_dict[i - 1][j].pos[1] - ball_size * sqrt(3);
				ball_dict[i][j].pos[2] = 0;
			}
			ball_dict[i][j].ball_there = false;
			ball_dict[i][j].visit = false;
		}
	}
}

Ball createBall(float x, float y, float z)
{
	if (dropped_balls > 50)num_color = 6;
	else if (dropped_balls > 30)num_color = 5;
	
	Color c = (Color)randomColor(num_color);
	Ball ball(x, y, z, c);
	return ball;
}

void ballInit()
{
	ball_ready = createBall((b_left+b_right)/2, b_bottom, 0);
	ball_next = createBall(b_left, b_bottom, 0);

	ballDictInit();
}

void drawAxis()
{
	glColor3f(0, 0, 0);
	glLineWidth(2.0f);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(10, 0xAAAA);
	glBegin(GL_LINES);
		glVertex3f(300, 0, 0);
		glVertex3f(-300, 0, 0);
		glVertex3f(0, 300, 0);
		glVertex3f(0, -300, 0);
		glVertex3f(0, 0, 300);
		glVertex3f(0, 0, -300);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

void drawBorder()
{
	glColor3f(1, 1, 1);
	glLineWidth(5.0f);
	glBegin(GL_LINES);
		glVertex3f(b_left-ball_size, b_bottom, 0);
		glVertex3f(b_left-ball_size, b_top+ball_size, 0);
		glVertex3f(b_right+ball_size, b_bottom, 0);
		glVertex3f(b_right+ball_size, b_top+ball_size, 0);
		glVertex3f(b_left-ball_size, b_bottom, 0);
		glVertex3f(b_right+ball_size, b_bottom, 0);
		glVertex3f(b_left-ball_size, b_top+ball_size, 0);
		glVertex3f(b_right+ball_size, b_top+ball_size, 0);
	glEnd();
}

void drawCase()
{
	//tight fit: 1.45
	float r = ball_size * 2;
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, case_tx.getTextureID());

	glPushMatrix();

	glPushMatrix();
	glTranslatef(b_left + r / 10, b_bottom - r / 35, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-r, -r, 0);
		glTexCoord2f(1, 0);
		glVertex3f(r, -r, 0);
		glTexCoord2f(1, 1);
		glVertex3f(r, r, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-r, r, 0);
	glEnd();
	glPopMatrix();

	glTranslatef((b_left + b_right) / 2 + r / 10, b_bottom - r / 35, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
		glVertex3f(-r, -r, 0);
		glTexCoord2f(1, 0);
		glVertex3f(r, -r, 0);
		glTexCoord2f(1, 1);
		glVertex3f(r, r, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-r, r, 0);
	glEnd();

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glColor3f(0, 1, 0);
	glPointSize(3.0f);

	glTranslatef((b_left + b_right) / 2, b_bottom, 0);
	glBegin(GL_POINTS);
	r *= 0.95;
	const int point_num = 100;
	for (int i = 0; i < point_num*time_left/autoLaunchLimit; i++) {
		glVertex2f(r * cos(-M_PI_4 * 0.8 + M_PI_2 * i / point_num), r * sin(-M_PI_4 * 0.8 + M_PI_2 * i / point_num));
	}
	glEnd();

	glPopMatrix();
}

void drawArrow()
{
	glEnable(GL_TEXTURE_2D);
	
	float r = ball_size;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();

	glTranslatef((b_left + b_right) / 2, b_bottom, 0);
	glRotatef(angle - 90, 0, 0, 1);

	glPushMatrix();
	glTranslatef(0, r, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, arrow_tx_2.getTextureID());
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex3f(-r, -r, 0);
		glTexCoord2f(0, 0);
		glVertex3f(r, -r, 0);
		glTexCoord2f(0, 1);
		glVertex3f(r, 2 * r, 0);
		glTexCoord2f(1, 1);
		glVertex3f(-r, 2 * r, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 4 * r, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, arrow_tx_1.getTextureID());
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex3f(-r, -r, 0);
		glTexCoord2f(0, 0);
		glVertex3f(r, -r, 0);
		glTexCoord2f(0, 1);
		glVertex3f(r, r, 0);
		glTexCoord2f(1, 1);
		glVertex3f(-r, r, 0);
	glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);
	glPopMatrix();
	/*
	glColor3f(0, 1, 0);
	glLineWidth(10.0f);
	glBegin(GL_LINES);
		//glVertex3f((b_left + b_right) / 2 + 300 * cos(ang_rad), b_bottom + 300 * sin(ang_rad), 0);
		//glVertex3f((b_left + b_right) / 2, b_bottom , 0);
		glVertex3f((b_left + b_right) / 2 + r * cos(ang_rad), b_bottom + r * sin(ang_rad), 0);
		glVertex3f((b_left + b_right) / 2 + 0.7 * r * cos(ang_rad + 0.3), b_bottom + 0.7 * r * sin(ang_rad + 0.3), 0);
		glVertex3f((b_left + b_right) / 2 + r * cos(ang_rad), b_bottom + r * sin(ang_rad), 0);
		glVertex3f((b_left + b_right) / 2 + 0.7 * r * cos(ang_rad - 0.3), b_bottom + 0.7 * r * sin(ang_rad - 0.3), 0);
	glEnd();
	*/
	glDisable(GL_TEXTURE_2D);
}



void drawRoom()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, floor_tx.getTextureID());
	glBegin(GL_QUADS);
	float left_end = -b_right - ball_size;
	float right_end = b_right + ball_size;

	//floor
	float width = right_end - left_end;
	for (int i = 0; i < 3; i++) {
			glTexCoord2f(0, 0);
			glVertex3f(left_end + width * i / 3, b_bottom, 0);
			glTexCoord2f(0, 1);
			glVertex3f(left_end + width * (i + 1) / 3, b_bottom, 0);
			glTexCoord2f(1, 1);
			glVertex3f(left_end + width * (i + 1) / 3, b_bottom, 300);
			glTexCoord2f(1, 0);
			glVertex3f(left_end + width * i / 3, b_bottom, 300);
	}
	glEnd();
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, wall_tx.getTextureID());
	
	//right wall
	float height = b_top - b_bottom + ball_size + 5;
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(right_end, b_bottom, 0);
		glTexCoord2f(0, 1);
		glVertex3f(right_end, b_bottom + height,0);
		glTexCoord2f(1, 1);
		glVertex3f(right_end, b_bottom + height, 400);
		glTexCoord2f(1, 0);
		glVertex3f(right_end, b_bottom, 400);
	glEnd();

	//wall
	glBegin(GL_QUADS);
	for (int i = 0; i < 2; i++) {
		glTexCoord2f(0, 0);
		glVertex3f(left_end + width * i / 2, b_bottom, 0);
		glTexCoord2f(0, 1);
		glVertex3f(left_end + width * (i + 1) / 2, b_bottom, 0);
		glTexCoord2f(1, 1);
		glVertex3f(left_end + width * (i + 1) / 2, b_bottom + height, 0);
		glTexCoord2f(1, 0);
		glVertex3f(left_end + width * i / 2, b_bottom + height, 0);
	}
	glEnd();

	//left_wall
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(left_end, b_bottom, 0);
		glTexCoord2f(0, 1);
		glVertex3f(left_end, b_bottom + height, 0);
		glTexCoord2f(1, 1);
		glVertex3f(left_end, b_bottom + height, 400);
		glTexCoord2f(1, 0);
		glVertex3f(left_end, b_bottom, 400);
	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, board_tx.getTextureID());

	glBegin(GL_QUADS);
		glTexCoord2f(0.32, 0.635);
		glVertex3f(b_left-ball_size, b_bottom, 0);
		glTexCoord2f(0.5, 0.46);
		glVertex3f(b_right + ball_size, b_bottom, 0);
		glTexCoord2f(0.68, 0.64);
		glVertex3f(b_right + ball_size, b_top + ball_size, 0);
		glTexCoord2f(0.54, 0.83);
		glVertex3f(b_left - ball_size, b_top + ball_size, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawScoreBoard()
{
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, leaderboard_tx.getTextureID());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0.03);
		glVertex3f(lb_left - 25, -50, 0);
		glTexCoord2f(1, 0.03);
		glVertex3f(b_left-ball_size-5, -50, 0);
		glTexCoord2f(1, 0.97);
		glVertex3f(b_left-ball_size-5, b_top - 5, 0);
		glTexCoord2f(0, 0.97);
		glVertex3f(lb_left - 25, b_top - 5, 0);
		for (int i = 0; i < 5; i++) {
			const float* pos = leaderboard_pos[i].getCoor();
			glTexCoord2f(0, 0.03);
			glVertex3f(pos[0], pos[1] - lb_height, 0);
			glTexCoord2f(1, 0.03);
			glVertex3f(pos[0] + 100, pos[1] - lb_height, 0);
			glTexCoord2f(1, 0.97);
			glVertex3f(pos[0] + 100, pos[1], 0);
			glTexCoord2f(0, 0.97);
			glVertex3f(pos[0], pos[1], 0);
		}
	glEnd();

	for (int i = 0; i < 5; i++) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, emblem_tx[4 - i].getTextureID());
		const float* pos = leaderboard_pos[i].getCoor();
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(pos[0], pos[1] - lb_height, 0);
			glTexCoord2f(1, 0);
			glVertex3f(pos[0] + lb_height, pos[1] - lb_height, 0);
			glTexCoord2f(1, 1);
			glVertex3f(pos[0] + lb_height, pos[1], 0);
			glTexCoord2f(0, 1);
			glVertex3f(pos[0], pos[1], 0);
		glEnd();
	}

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}

void lightInit()
{
	GLfloat light_position[] = { 0,0,-1,0 };
	GLfloat light_ambient[] = { 1,1,1,1 };
	GLfloat light_diffuse[] = { 1,1,1,1 };
	GLfloat light_specular[] = { 1,1,1,1 };

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
}

void drawBalls()
{
	ball_ready.draw();
	ball_next.draw();
	for (Ball ball : balls) {
		if (ball.getVisible()) ball.draw();
	}
	for (Ball ball : dropping_balls) {
		ball.draw();
	}
}

void moveBall()
{
	float x = ball_ready.getx();
	float y = ball_ready.gety();
	float z = ball_ready.getz();
	float Vx = ball_ready.getVx();
	float Vy = ball_ready.getVy();
	time_left -= 0.02;
	if (time_left <= 0) {
		animator.startSwing();
		ball_ready.setV(v * cos(ang_rad), v * sin(ang_rad));
		time_left = autoLaunchLimit;
	}
	if (Vx != 0 || Vy != 0) {
		ball_ready.setPos(x + Vx, y + Vy, z);
		//cout << "x: " << x + Vx << " y: " << y + Vy << endl;
		if (ball_ready.getx() < b_left || ball_ready.getx() > b_right) ball_ready.setV(-Vx, Vy);
		if (ballStop(ball_ready)) {
			ball_ready.setV(0, 0);
			balls.push_back(ball_ready);
			ball_ready = ball_next;
			ball_ready.setPos((b_left + b_right) / 2, b_bottom, 0);
			ball_next = createBall(b_left, b_bottom, 0);
			mainCalc();
		}
	}
	if (dropping_balls.size() > 0) {
		for (auto ball = dropping_balls.begin(); ball < dropping_balls.end();) {
			ball->setPos(ball->getx() + ball->getVx(), ball->gety() + ball->getVy(), ball->getz());
			ball->setV(ball->getVx(), ball->getVy() - v / 100);
			if (dropStop(*ball)) {
				//cout << "stop ball" << endl;
				ball = dropping_balls.erase(ball);
			}
			else ball++;
		}
	}
}

void writeText(Vector3f color, Vector2f pos, string text)
{
	const float* colorv = color.getCoor();
	const float* posv = pos.getCoor();
	glColor3fv(colorv);
	glRasterPos2fv(posv);
	for (auto i : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, i);
	}
}

void displayScore() 
{
	cur_score = dropped_balls * 100;
	for (int i = 0; i < 4; i++) {
		const float* pos = leaderboard_pos[i].getCoor();
		string sc = to_string(score[i]);
		writeText(Vector3f(1, 1, 1), Vector2f(pos[0] + 55 - 2.5 * (sc.size() - 1), pos[1] - lb_height / 2 - 2), sc);
	}
	const float* pos = leaderboard_pos[4].getCoor();
	string sc = to_string(cur_score);
	writeText(Vector3f(1, 1, 1), Vector2f(pos[0] + 55 - 2.5 * (sc.size() - 1), pos[1] - lb_height / 2 - 2), sc);
}

void displayGameOver()
{
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
		glVertex2f(-400, -300);
		glVertex2f(400, -300);
		glVertex2f(400, 300);
		glVertex2f(-400, 300);
	glEnd();
	
	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, game_over_tx.getTextureID());
	
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(-200, -150);
		glTexCoord2f(1, 0);
		glVertex2f(200, -150);
		glTexCoord2f(1, 1);
		glVertex2f(200, 150);
		glTexCoord2f(0, 1);
		glVertex2f(-200, 150);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, restart_tx.getTextureID());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(-250, -150);
		glTexCoord2f(1, 0);
		glVertex2f(-200, -150);
		glTexCoord2f(1, 1);
		glVertex2f(-200, -100);
		glTexCoord2f(0, 1);
		glVertex2f(-250, -100);
	glEnd();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, close_tx.getTextureID());

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(200, -150);
		glTexCoord2f(1, 0);
		glVertex2f(250, -150);
		glTexCoord2f(1, 1);
		glVertex2f(250, -100);
		glTexCoord2f(0, 1);
		glVertex2f(200, -100);
	glEnd();

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}

void renderScene()
{
	//color, projection
	glClearColor(0.5, 0.5, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1280 / 720.0, -10, 100);

	//camera, light
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 450,
		0, 0, 0,
		0, 1, 0);
	lightInit();

	//draw 2D
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	drawRoom();
	drawScoreBoard();
	//drawBorder();
	drawCase();
	drawArrow();
	//drawAxis();

	displayScore();
	writeText(Vector3f(1, 1, 1), Vector2f(210, -170), "Designed by. Jihwan Kim");

	//draw 3D
	if (!pause) {
		animator.update();
		moveBall();
	}
	
	drawBalls();
	
	glPushMatrix();

	glTranslatef(-100, -100, 100);
	glRotatef(180, 0, 1, 0);
	glRotatef(15, 1, 0, 0);
	
	animator.obj.draw();

	glPopMatrix();
	
	if (game_over) {
		pause = true;
		displayGameOver();
	}

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	if (key == ' ') {
		//pause = !pause;
		if (!pause) {
			animator.startSwing();
			if (ball_ready.getVx() * ball_ready.getVy() == 0) {
				time_left = autoLaunchLimit;
				ball_ready.setV(v * cos(ang_rad), v * sin(ang_rad));
			}
		}
	}
	//esc 27
	else if (key == 27) {
		pause = !pause;
	}
}

void processSpecialKeys(int key, int x, int y) {
	if (!pause) {
		switch (key)
		{
		case GLUT_KEY_LEFT:
			/*animator.setAnimationTime(animator.getAnimationTime() - 0.09);
			animator.update();
			break;*/
			if (angle <= 167) angle += 3; ang_rad = angle * M_PI / 180; break;
		case GLUT_KEY_RIGHT:
			/*animator.setAnimationTime(animator.getAnimationTime() + 0.03);
			animator.update();
			break;*/
			if (angle >= 13) angle -= 3; ang_rad = angle * M_PI / 180; break;
		default:
			break;
		}
	}
}

void processMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && game_over) {
		int real_x, real_y;
		real_y = (-y + HEIGHT / 2) / 2;
		real_x = (x - WIDTH / 2) / 2;
		//cout << "x: " << real_x << " y: " << real_y << endl;

		if (-250 < real_x && real_x < -200 && -150 < real_y && real_y < 150) {
			animator.setAnimationTime(0);
			balls.clear();
			dropping_balls.clear();
			dropped_balls = 0;
			for (int i = 0; i < ball_row; i++) delete[] ball_dict[i];
			delete[] ball_dict;
			ballInit();
			game_over = false;
			pause = false;
			ofstream score_out("score.txt");
			score.push_back(cur_score);
			sort(score.begin(), score.end());
			for (int i = score.size() - 1; i >= score.size() - 4; i--) {
				score_out << score[i];
				score_out << endl;
			}
			score_out.close();
			score.clear();
			score.resize(4, 0);
			scoreInit();
		}

		else if (200 < real_x && real_x < 250 && -150 < real_y && real_y < 150) {
			ofstream score_out("score.txt");
			score.push_back(cur_score);
			sort(score.begin(), score.end());
			for (int i = score.size() - 1; i >= score.size() - 4; i--) {
				score_out << score[i];
				score_out << endl;
			}
			score_out.close();
			for (int i = 0; i < ball_row; i++) delete[] ball_dict[i];
			delete[] ball_dict;
			//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			exit(0);
		}
	}
}

void main(int argc, char** argv)
{
	ballInit();
	scoreInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Veigar");
	init();

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouse);

	glutMainLoop();
}