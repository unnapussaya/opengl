//  ========================================================================
//  COSC363: Computer Graphics (2013);  University of Canterbury.
//	Name: Unna Pussayapaiboon
//  Student ID: 64462335
//  FILE NAME: PaintingFactory.cpp
//  
//  ========================================================================


#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include "paintFactory.h"
#include "spotlight.h"
#include "conveyor.h"
#include "drawObject.h"

GLUquadric *q = gluNewQuadric();

//=================== GLOBAL VARIABLES ======================

float angleGear1 = 0, angleGear2 = 0;
float angle_cannon = -180, angle_camera = 0;

float ex = 0, ey = 70, ez = 400;
float ly = 10, r = 400;

float doorUp = 20, doorDown = 0;

float beltTranslation[43], belty[43], beltRotation[43];

float col[3] = { 0.2, 0.2, 0.2 };

float teapot_x = -COV_LEN / 2, teapot_y = 58, teapot_rot = 0;     //teapot's location & angle
float torus_x = COV_LEN - 30, torus_rot = 0;                    //torus's location & angle

float spotlightCol[4] = { 0.94, 0.5, 0.5, 1. };
float teapotCol[4] = { 0.94, 0.5, 0.5, 1. };
float torusCol[4] = {};

float ball_x = 0, ball_y = 0, y_inc = 2;                 //ball's location

 //------------switch condition----------------
bool reset_flag = false, pushback_flag = false, cannonReverse_flag = false, cannonActive_flag = false;
int shoot_state = 0, currentCol_state = 0;



//=============================================================================
//=================== TIMER FUNCTION ======================


//------------- Teapot Timer ---------------
void teapot_timer() {
	int i = 0;

	//change its colour
	if (teapot_x > 4) {
		if (col[0] < teapotCol[0]) col[0] += 0.01;
		if (col[1] < teapotCol[1]) col[1] += 0.01;
		if (col[2] < teapotCol[2]) col[2] += 0.01;
	}

	//moving along the conveyor path	
	if (teapot_x < COV_LEN / 2) teapot_x++;
	else if (teapot_y > 20 && teapot_rot > -70) {

		if (teapot_rot <= -5) {
			teapot_x += 1;
		}
		else {
			teapot_y--;
			teapot_x += 2;
		}

		cannonActive_flag = true;
		teapot_rot--;
		if (teapot_x > COV_LEN - 35) {
			teapot_x = COV_LEN - 35;

			//Change the colour of the torus to be the same as teapot
			for (i = 0; i < 4; i++) {
				torusCol[i] = teapotCol[i];
			}
		}
	}

	//reset_flag teapot
	if (reset_flag) {
		col[0] = 0.2;
		col[1] = 0.2;
		col[2] = 0.2;
		teapot_rot = 0;
		teapot_x = -COV_LEN / 2;
		teapot_y = 58;
		reset_flag = false;
		for (i = 0; i < 4; i++) {
			teapotCol[i] = spotlightCol[i];
		}
	}
}


//------------- Door Timer ---------------
void door_timer() {

	//Door lift TOP_OFFSET, gear rotate
	if (teapot_x == COV_LEN - 35 && doorUp < 45) {
		angleGear2 += 4;
		doorUp++;
		if (doorUp == 45) reset_flag = true;
	}

	//Release torus
	else if (doorUp >= 45 && doorUp < 55) {
		doorUp++;
		pushback_flag = true;
	}

	//Door move down, gear rotate in the opp direction
	else if (doorUp == 55 && doorDown < 35) {
		angleGear2 -= 4;
		doorDown++;
	}

	//Door back to initial state
	else {
		doorUp = 20;
		doorDown = 0;
	}
}


//------------- Torus Timer ---------------
void torus_timer() {

	//Torus move
	if (pushback_flag && torus_x != -COV_LEN / 2 - 20) {
		torus_rot++;
		torus_x -= 1.5;
	}

	//reset_flag torus
	if (torus_x <= -COV_LEN / 2 - 20) {
		currentCol_state++;
		if (currentCol_state > 8) currentCol_state = 0;
		pushback_flag = false;
		torus_x = COV_LEN - 30;
	}
}


//------------- Cannon Timer ---------------
void cannon_timer() {
	if (cannonActive_flag) {

		//Cannon rotate TOP_OFFSET
		if (angle_cannon > -270 && cannonReverse_flag == 0 && shoot_state != 1) {
			angle_cannon--;
			if (angle_cannon == -270) cannonReverse_flag = 1;
		}

		//shoot_state the ball when it's 45 degree
		if (angle_cannon == -225 && shoot_state == 0 && !cannonReverse_flag) {
			shoot_state = 1;
		}

		//Cannon rotate down
		if (cannonReverse_flag && shoot_state != 1) {
			angle_cannon++;
			if (angle_cannon == -180) {
				cannonReverse_flag = false;
				cannonActive_flag = 0;
			}
		}
	}

	if (shoot_state == 1) {

		//State1::  ball lanching and hasn't reached the side of the machin
		if (ball_x > -(COV_LEN - 40)) {
			ball_x -= 2;
			ball_y += y_inc;

			if (ball_y > 25) {
				y_inc *= 0.97;
				if (y_inc < 0.01) y_inc *= -1;
			}
		}
		else {
			shoot_state = 2;
			y_inc = -0.01;
		}
	}

	//State2::  collide and bounching back
	if (shoot_state == 2) {
		ball_x += 0.36;
		ball_y += y_inc;
		y_inc -= 0.03;
	}

	//State3::  rolling down to the machin
	if (ball_y <= 46 && (shoot_state == 2 || shoot_state == 3)) {
		shoot_state = 3;
		ball_x--;
		ball_y -= 0.3;
	}

	//State4::  inside the machine so reset_flag back to be inside the cannon
	if (ball_x <= -(COV_LEN - 25)) {
		shoot_state = 0;
		ball_x = 0;
		ball_y = 0;
		y_inc = 2;
	}
}


//------------- Main Timer ---------------
void myTimer(int value) {
	//run animation for 1 minute.		
	if (value < 600) {

		//update scene parameters
		if (angleGear1 < -360) {
			angleGear1 = 0;
		}

		glutPostRedisplay();
		angleGear1 -= 6;

		teapot_timer();
		door_timer();
		torus_timer();
		cannon_timer();

		glutTimerFunc(20, myTimer, value);
	}
}

//=============================================================================
//=================== INITIALIZE ======================

void initialize(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	gluQuadricDrawStyle(q, GLU_FILL);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60., 1.0, 10.0, 1000.0);   //Perspective projection

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, GREY);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}


//=============================================================================
//=================== DISPLAY FUNCTION ======================


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(ex, ey, ez, ex - r*sin(angle_camera), ly, ez - r*cos(angle_camera), 0.0, 1.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, MAIN_LIGHT_POS);

	spot_light(currentCol_state, spotlightCol);
	floor();

	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);

	float spotdir[4] = { 0, 10, 0 };  //spotlight point

	glLightfv(GL_LIGHT1, GL_POSITION, SPOTLIGHT_POS);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);

	draw_machine(beltTranslation, beltRotation, angleGear1, angleGear2);
	draw_allboxes(doorUp, doorDown);
	draw_cannon(angle_cannon, spotlightCol, ball_x, ball_y);
	drawTeapot(teapot_x, teapot_y, teapot_rot, col);
	drawTorus(torus_x, torus_rot, torusCol);

	glutSwapBuffers();
}

void special(int key, int x, int y) {
	//zoom out
	if (key == GLUT_KEY_DOWN) {
		ex += CAM_ZOOM_FACTOR * sin(angle_camera);
		ez += CAM_ZOOM_FACTOR * cos(angle_camera);
	}

	//zoom in
	else if (key == GLUT_KEY_UP) {
		ex -= CAM_ZOOM_FACTOR * sin(angle_camera);
		ez -= CAM_ZOOM_FACTOR * cos(angle_camera);
	}

	//rotate left 5 degree
	else if (key == GLUT_KEY_LEFT) angle_camera += CAM_ROT_FACTOR * DEG2RAD;

	//rotate right 5 degree
	else if (key == GLUT_KEY_RIGHT) angle_camera -= CAM_ROT_FACTOR * DEG2RAD;

	glutPostRedisplay();
}



//=============================================================================
//=================== MAIN FUNCTION ======================


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(argv[0]);

	initialize();
	block_position(beltTranslation, beltRotation);
	glutTimerFunc(100, myTimer, 0);
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutMainLoop();

	return 0;
}
