//  ========================================================================
//  COSC363: Computer Graphics (2013);  University of Canterbury.
//	Name: Unna Pussayapaiboon
//  Student ID: 64462335
//  FILE NAME: Cylinder.cpp
//  
//  Cylinder Class
//  ========================================================================

#include "Cylinder.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/

bool Cylinder::checkTall (Vector pos, Vector dir, float t) {
	float y_max = center.y+tall;
	
	Vector q = pos + dir*t;	
	if (q.y >= center.y && q.y <= y_max) return 1;

	else return 0;
	
}


float Cylinder::intersect(Vector pos, Vector dir) {
	Vector vdif = pos - center;
	
	float a = (dir.x * dir.x) + (dir.z * dir.z);
	float b = 2 * (dir.x * vdif.x + dir.z * vdif.z);
	float c = (vdif.x) * (vdif.x) + (vdif.z) * (vdif.z) - (radius * radius);
	
	float num = (b * b) - (4 * a * c);
	if(fabs (num) < 0.001) return -1.0; 
	if(num < 0.0) return -1.0;

	
	float t1 = (-b -sqrt(num)) / (2 * a);
	float t2 = (-b + sqrt(num)) / (2 * a);
	

	if (fabs(t1) < 0.001 )
	{
		if (t2 > 0){
			if (checkTall (pos, dir, t2)) return t2;
			else return -1.0;
		}
		else t1 = -1.0;
	}
	if(fabs(t2) < 0.001 ) t2 = -1.0;

	if(t1 < t2){
		if (checkTall (pos, dir, t1)) return t1;
		else if (checkTall (pos, dir, t2)) return t2;
		else return -1.0;
	}
	else{
		if (checkTall (pos, dir, t2)) return t2;
		else if (checkTall (pos, dir, t1)) return t1;
		else return -1.0;	
	}
}


/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
Vector Cylinder::normal (Vector p) {
    Vector n = p - center;
    n.y = 0;
    n.normalise ();
    return n;
}
