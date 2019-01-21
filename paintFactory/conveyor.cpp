#include <math.h>
#include <GL/glut.h>
#include "paintFactory.h"
#include "gear.h"


//=============================================================================
//=================== CONVEYER'S BLOCK ======================

//------------- Block position ---------------
void block_position(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER]) {
	int i = 0;
	for (i = 0; i < BLOCK_NUMBER; i++) {
		//Top conveyor
		if (i < (((BLOCK_NUMBER - 1) / 2) - 2)) {
			beltTranslation[i] = (i * CONVEYOR_BLOCK_GAP);
			beltRotation[i] = 0;
		}

		//Bottom conveyor
		else if (i >(BLOCK_NUMBER - 1) / 2 && i < (BLOCK_NUMBER - 3)) {
			beltTranslation[i] = beltTranslation[i - 1] - CONVEYOR_BLOCK_GAP;
			beltRotation[i] = -180;
		}

		else {
			//Right conveyor
			if (i >= (((BLOCK_NUMBER - 1) / 2) - 2) && i <= (BLOCK_NUMBER - 1) / 2) {
				beltTranslation[i] = beltTranslation[i - 1];
			}

			//Left conveyor
			else {
				beltTranslation[i] = 0;
			}

			beltRotation[i] = beltRotation[i - 1] - 60;
		}
	}
}


void move_block(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER]) {
	int i = 0;
	glTranslatef(-COV_LEN / 2, 0.0, 0.0);
	for (i = 0; i < BLOCK_NUMBER; i++) {
		glPushMatrix();

		//Move forwards (top of conveyor)
		if (beltTranslation[i] < COV_LEN && beltRotation[i] == 0) beltTranslation[i]++;

		//Rotate 180 degree clockwise  (top->bottom)
		else if (beltTranslation[i] == COV_LEN && beltRotation[i] > -180) beltRotation[i] -= 6;

		//Move forwards (bottom of conveyor)
		else if (beltRotation[i] == -180 && beltTranslation[i] != 0) beltTranslation[i]--;

		//Rotate 180 degree clock wise  (bottom->top)
		else if (beltTranslation[i] == 0 && beltRotation[i] > -360) beltRotation[i] -= 6;

		//reset_flag the angle
		if (beltRotation[i] <= -360) beltRotation[i] = 0;

		glTranslatef(beltTranslation[i], 0, 0.0);
		glTranslatef(0.0, -10.0, 0.0);
		glRotatef(beltRotation[i], 0, 0, 1);
		glTranslatef(0.0, 10.0, 0.0);
		glScalef(COV_LEN / 20, 2, COV_WIDTH);
		glutSolidCube(1.0);

		glPopMatrix();
	}
}


//------------- Sidebelt ---------------
void drawSidebelt(float radius, float len, float width, int min) {
	float radius_x[180], radius_y[180];

	int i = min;
	int max = min + len;
	glBegin(GL_LINE_LOOP);
	for (i = min; i < max; i++) {
		float degInRad = i*DEG2RAD;

		radius_x[i - min] = cos(degInRad) * radius;
		radius_y[i - min] = sin(degInRad) * radius;
	}
	glEnd();

	glBegin(GL_QUADS);
	for (i = 0; i < (len - 1); i++) {
		glVertex3f(radius_x[i], radius_y[i], 0);
		glVertex3f(radius_x[i + 1], radius_y[i + 1], 0);
		glVertex3f(radius_x[i + 1], radius_y[i + 1], -width);
		glVertex3f(radius_x[i], radius_y[i], -width);
	}
	glEnd();
}


//------------- Conveyor's legs ---------------
void draw_conv_leg() {
	const int convLeg_length = 22;

	glPushMatrix();
	glTranslatef(COV_LEN / 2, -TOP_OFFSET / 2, convLeg_length);
	glScalef(5, TOP_OFFSET, 4);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(COV_LEN / 2, -TOP_OFFSET / 2, -convLeg_length);
	glScalef(5, TOP_OFFSET, 4);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-COV_LEN / 2, -TOP_OFFSET / 2, convLeg_length);
	glScalef(5, TOP_OFFSET, 4);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-COV_LEN / 2, -TOP_OFFSET / 2, -convLeg_length);
	glScalef(5, TOP_OFFSET, 4);
	glutSolidCube(1.0);
	glPopMatrix();

}

//-----------------Conveyor-----------------
void conveyor(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER]) {
	int conv_radius = 10;
	glColor4f(0.2, 0.2, 0.2, 1.0);

	glPushMatrix();
	glScalef(COV_LEN, COV_THICKNESS, COV_WIDTH);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -(2 * conv_radius), 0.0);
	glScalef(COV_LEN, COV_THICKNESS, COV_WIDTH);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(COV_LEN / 2 - 1, -conv_radius, COV_WIDTH / 2);
	drawSidebelt(conv_radius, COV_LEN, COV_WIDTH, -90);

	glTranslatef(-COV_LEN + 2, 0, 0);
	drawSidebelt(conv_radius, COV_LEN, COV_WIDTH, 90);
	glPopMatrix();

	glPushMatrix();
	move_block(beltTranslation, beltRotation);
	glPopMatrix();

	draw_conv_leg();
}

//-----------------Box-----------------
void draw_box(float s) {
	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(s, s, s);
	glVertex3f(0, s, s);

	glVertex3f(s, 0, s);
	glVertex3f(0, 0, s);

	glVertex3f(s, 0, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(s, s, 0);
	glVertex3f(0, s, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, s);
	glVertex3f(0, s, s);
	glVertex3f(0, s, 0);
	glEnd();
	glPopMatrix();
}

void draw_machine(float beltTranslation[BLOCK_NUMBER], float beltRotation[BLOCK_NUMBER], float angleGear1, float angleGear2) {
	const int boxSize = 50;

	glPushMatrix();
	glTranslatef(0, TOP_OFFSET, 0.0);
	glPushMatrix();
	glTranslatef(-COV_LEN / 2 - boxSize, 0, -COV_WIDTH / 2 - 5);
	glColor4f(0.4, 0.4, 0.4, 1.0);
	draw_box(boxSize);
	glPushMatrix();
	glTranslatef(COV_WIDTH / 2, -10, COV_WIDTH / 2 + 5);
	glScalef(boxSize / 4, TOP_OFFSET / 2 - 5, boxSize / 4);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
	conveyor(beltTranslation, beltRotation);
	draw_allgears(angleGear1, angleGear2);
	glPopMatrix();

}

void draw_allboxes(float doorUp, float doorDown) {
	const int boxSize = 30;

	//colour changing box------------------------------
	glPushMatrix();
	glTranslatef(0, TOP_OFFSET * 4, 0);
	glScalef(20, 200, 20);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, TOP_OFFSET * 2 + 15, 0);
	glRotatef(15, 0, 0, 1);
	glScalef(35, 5, 5);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-boxSize / 2, TOP_OFFSET * 2 + 5, -boxSize / 2);
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_box(boxSize);

	glPopMatrix();
	//----------------------------------------------------

	//box for collecting torus
	glPushMatrix();
	glColor4f(0.2, 0.3, 0.4, 1.0);
	glTranslatef(-COV_LEN / 2 - 35, 15, 15);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, -15, 0);
	draw_box(boxSize);
	glPopMatrix();

	//recycle box
	glPushMatrix();
	glTranslatef(COV_LEN, -1, -22);
	glScalef(1, 1.7, 1);
	glRotatef(180, 0, 1, 0);

	glTranslatef(0, 22, 0);
	glRotatef(180, 1, 0, 0);
	glTranslatef(0, -22, 0);
	draw_box(44);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(COV_LEN - 44, doorUp - doorDown, 0);
	glScalef(1, 40, 44);
	glutSolidCube(1.0);
	glPopMatrix();
}