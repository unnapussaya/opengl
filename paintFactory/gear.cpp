#include <math.h>
#include <GL/glut.h>
#include "paintFactory.h"

float getRadius(int numTeeth) {
	return 1.4 * numTeeth / PI;
}

void drawGear(int numTeeth, float thinkness) {
	float X[25], X2[25], X3[25], X4[25];
	float Y[25], Y2[25], Y3[25], Y4[25];

	float radius = getRadius(numTeeth);
	float angInc1 = 0.5 / radius;
	float angInc2 = 0.8 / radius;
	float angle = 0.0;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0);

	for (int i = 0; i < numTeeth; i++) {
		X[i] = radius*cos(angle);
		Y[i] = radius*sin(angle);
		glVertex3f(X[i], Y[i], 0);
		angle += angInc1;

		X2[i] = (radius + 2.0) * cos(angle);
		Y2[i] = (radius + 2.0) * sin(angle);
		glVertex3f(X2[i], Y2[i], 0);
		angle += angInc2;

		X3[i] = (radius + 2.0) * cos(angle);
		Y3[i] = (radius + 2.0) * sin(angle);
		glVertex3f(X3[i], Y3[i], 0);
		angle += angInc1;

		X4[i] = radius * cos(angle);
		Y4[i] = radius * sin(angle);
		glVertex3f(X4[i], Y4[i], 0);

		angle += 2.0 * angInc1;
	}

	glVertex3f(radius, 0.0, 0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, thinkness);

	for (int i = 0; i < numTeeth; i++) {
		glVertex3f(radius*cos(angle), radius * sin(angle), thinkness);
		angle += angInc1;

		glVertex3f((radius + 2.0) * cos(angle), (radius + 2.0) * sin(angle), thinkness);
		angle += angInc2;

		glVertex3f((radius + 2.0) * cos(angle), (radius + 2.0) * sin(angle), thinkness);
		angle += angInc1;

		glVertex3f(radius * cos(angle), radius * sin(angle), thinkness);
		angle += 2.0*angInc1;

	}
	glVertex3f(radius, 0.0, thinkness);
	glEnd();

	angle = 0.0;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < numTeeth; i++) {
		glColor4f(0.5, 0.5, 0.5, 1.0);
		glVertex3f(X[i], Y[i], 0);
		glVertex3f(X[i], Y[i], thinkness);

		glVertex3f(X2[i], Y2[i], 0);
		glVertex3f(X2[i], Y2[i], thinkness);


		glVertex3f(X3[i], Y3[i], 0);
		glVertex3f(X3[i], Y3[i], thinkness);

		glVertex3f(X4[i], Y4[i], 0);
		glVertex3f(X4[i], Y4[i], thinkness);

	}
	glVertex3f(X[0], Y[0], 0);
	glVertex3f(X[0], Y[0], thinkness);
	glEnd();

}


float second_ang(int n1, int n2, float ang1) {
	return -(n1 * ang1) / n2;
}


void draw_allgears(float angleGear1, float angleGear2) {
	int gear1 = 15;
	const int gearThickness = 40;

	glPushMatrix();
	glTranslatef(90.0, -10, -20.0);
	glRotatef(angleGear1, 0, 0, 1);
	glColor4f(0.3, 0.3, 0.3, 1.0);
	drawGear(gear1, gearThickness);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-90.0, -10, -22.0);
	glRotatef(angleGear1, 0, 0, 1);
	glColor4f(0.3, 0.3, 0.3, 1.0);
	drawGear(gear1, gearThickness);
	glPopMatrix();


	//-----synchronise gear------------	
	int gear2 = 10;
	int R2 = getRadius(gear2);

	int gear3 = 18;
	int R3 = getRadius(gear3);
	float theta3 = PI / 4;
	float x3 = (R2 + R3 + 2.5) * cos(theta3), y3 = (R2 + R3 + 2.5) * sin(theta3);

	int gear4 = 8;
	int R4 = getRadius(gear4);
	float theta4 = PI / 4;
	float x4 = x3 + ((R4 + R3 + 2.5) * cos(theta4));
	float y4 = y3 - ((R4 + R3 + 2.5) * sin(theta4));

	glTranslatef(COV_LEN - 35, 0, 24);
	glPushMatrix();
	glRotatef(angleGear2, 0, 0, 1);
	glColor4f(0.0, 0.0, 1.0, 1.0);
	drawGear(gear2, 4);         //A gear with 25 teeth
	glPopMatrix();

	glPushMatrix();
	float angleGear3 = second_ang(gear2, gear3, angleGear2);
	glTranslatef(x3, y3, 0);
	glRotatef(angleGear3, 0, 0, 1);  //moving gear angle
	glRotatef(45, 0, 0, 1);     //gear's angle
	glColor4f(1.0, 0.0, 1.0, 1.0);
	drawGear(gear3, 4);
	glPopMatrix();

	glPushMatrix();
	float angleGear4 = second_ang(gear3, gear4, angleGear3);
	glTranslatef(x4, y4, 0);
	glRotatef(angleGear4, 0, 0, 1);
	glRotatef(20, 0, 0, 1);
	glColor4f(0.0, 1.0, 0.0, 1.0);
	drawGear(gear4, 4);
	glPopMatrix();

}
