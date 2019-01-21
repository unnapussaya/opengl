#include <GL/glut.h>
#include "paintFactory.h"


void floor() {
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLACK);   //set prevent floor reflecting
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);

	for (int i = -300; i < 300; i++) {
		for (int j = -300; j < 300; j++) {
			glVertex3i(i, 0.0, j);
			glVertex3i(i, 0.0, j + 1);
			glVertex3i(i + 1, 0.0, j + 1);
			glVertex3i(i + 1, 0.0, j);
		}
	}

	glEnd();
}

void drawTeapot(float teapot_x, float teapot_y, float teapot_rot, float col[3]) {
	const int teapotSize = 10;
	const int teapotTrans_y = 23;

	//------------------teapot---------------------------		
	glPushMatrix();
	glTranslatef(teapot_x, -teapotTrans_y + teapot_y, 0.0);
	glRotatef(teapot_rot, 0, 0, 1);
	glTranslatef(-teapot_x, teapotTrans_y - teapot_y, 0.0);
	glTranslatef(teapot_x, teapot_y, 0.0);
	glColor4f(col[0], col[1], col[2], 1.0);
	glutSolidTeapot(teapotSize);
	glPopMatrix();
}

void drawTorus(float torus_x, float torus_rot, float torusCol[3]) {
	glPushMatrix();
	glColor4f(torusCol[0], torusCol[1], torusCol[2], 1.0);
	glTranslatef(torus_x, 12, 0.0);
	glRotatef(torus_rot, 0, 0, 1);
	glutSolidTorus(4, 6, 20, 20);
	glPopMatrix();
}

//-----------------Draw Cannon-----------------
void draw_cannon(float angle_cannon, float spotlightCol[3], float ball_x, float ball_y) {
	glPushMatrix();
	glTranslatef(COV_LEN - 25, 80, 0.0);

	glPushMatrix();
	glRotatef(angle_cannon, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	GLUquadric *q = gluNewQuadric();
	gluCylinder(q, 5, 5, 30, 20, 20);
	gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();

	glPushMatrix();
	glScalef(12, 12, 12);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor4f(spotlightCol[0], spotlightCol[1], spotlightCol[2], 1.0);
	glTranslatef(ball_x, ball_y, 0.0);
	glutSolidSphere(4, 20, 20);
	glPopMatrix();

	glPopMatrix();
}