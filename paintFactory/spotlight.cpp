#include <GL/glut.h>
#include "paintFactory.h"

void array_copy(float array1[4], const float array2[4]) {

	for (int i = 0; i < 4; i++) {
		array1[i] = array2[i];
	}
}

//------------------Change spotlight's colour------------------
void change_colour(int col, float my_object[4]) {
	if (col == 0) array_copy(my_object, LIGHT_CORAL);
	else if (col == 1) array_copy(my_object, GREEN);
	else if (col == 2) array_copy(my_object, SEA_GREEN);
	else if (col == 3) array_copy(my_object, LIGHT_BLUE);
	else if (col == 4) array_copy(my_object, VIOLET);
	else if (col == 5) array_copy(my_object, LIGHT_BROWN);
	else if (col == 6) array_copy(my_object, CHOCOLATE_BROWN);
	else if (col == 7) array_copy(my_object, GOLD);
	else if (col == 8) array_copy(my_object, PINK);

}


void spot_light(int currentCol_state, float spotlightCol[4]) {
	change_colour(currentCol_state, spotlightCol);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, spotlightCol);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT1, GL_SPECULAR, WHITE);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);

}
