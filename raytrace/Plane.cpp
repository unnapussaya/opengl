/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Plane.h"
#include "Vector.h"
#include <math.h>

//Function to test if an input point is within the quad.
bool Plane::isInside(Vector q)
{
	Vector n = normal(q);
	Vector ua = b-a,  ub = c-b, uc = d-c, ud = a-d;
	Vector va = q-a,  vb = q-b, vc = q-c, vd = q-d;
	//Complete this function
	
	float A = (ua.cross(va)).dot(n);
	float B = (ub.cross(vb)).dot(n);
	float C = (uc.cross(vc)).dot(n);
	float D = (ud.cross(vd)).dot(n);
	
	if (A>0 && B>0 && C>0 && D>0) return 1;
	else return 0;

}

//Function to compute the paramter t at the point of intersection.
float Plane::intersect(Vector pos, Vector dir)
{
	Vector n = normal(pos);
	Vector vdif = a-pos;
	float vdotn = dir.dot(n);
	if(fabs(vdotn) < 1.e-4) return -1;
    float t = vdif.dot(n)/vdotn;
	if(fabs(t) < 0.0001) return -1;
	Vector q = pos + dir*t;
	if(isInside(q)) return t;
    else return -1;
}

// Function to compute the unit nor// Function to compute the unit normal vector
// Remember to output a normalised vector!
Vector Plane::normal(Vector pos)
{
	Vector n= (b-a).cross(c-a);
	n.normalise();
	return n;
}



