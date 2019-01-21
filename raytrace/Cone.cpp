//  ========================================================================
//  COSC363: Computer Graphics (2013);  University of Canterbury.
//	Name: Unna Pussayapaiboon
//  Student ID: 64462335
//  FILE NAME: Cone.cpp
//  
//  Cone Class
//  ========================================================================

#include "Cone.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/

bool Cone::checkTall (Vector pos, Vector dir, float t) {
	Vector center2 = center;
	center2.y += tall;
	
	Vector q = pos + dir*t;	
	if (q.y >= center.y && q.y <= center2.y) return 1;

	else return 0;
	
}


float Cone::intersect(Vector pos, Vector dir)
{
	Vector vdif = pos - center;
	float cone_factor = (radius * radius) / tall;

	float a = (dir.x * dir.x) + (dir.z * dir.z) -(dir.y * dir.y) * cone_factor ;
	float b = 2 * (dir.x * vdif.x + dir.z * vdif.z + dir.y * cone_factor * (tall - pos.y + center.y));
	float c = (vdif.x) * (vdif.x) + (vdif.z) * (vdif.z) - (tall - pos.y + center.y) * (tall - pos.y + center.y) * cone_factor ;

	float num = (b * b) - (4 * a * c);
	if(fabs(num) < 0.001) return -1.0; 
	if(num < 0.0) return -1.0;

	
	float t1 = (-b -sqrt(num)) / (2 * a);
	float t2 = (-b +sqrt(num)) / (2 * a);
	
	if(fabs(t1) < 0.001 )
	{
		if (t2 > 0){
			if (checkTall (pos, dir, t2)) return t2;
		}
		else t1 = -1.0;
	}
	if(fabs(t2) < 0.001 ) t2 = -1.0;
	float t;
	if(t1 < t2) t = t1;
	else t = t2;
	
	if (checkTall (pos, dir, t)) return t;
	else return -1;
}


/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
Vector Cone::normal (Vector p) {
	float tan_delta = radius / tall;
    Vector n = p - center;

	float small_r = sqrt(n.x * n.x + n.z * n.z);
    n.y = small_r * tan_delta;

    n.normalise ();
    return n;
}
