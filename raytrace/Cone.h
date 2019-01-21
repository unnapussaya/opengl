//  ========================================================================
//  COSC363: Computer Graphics (2013);  University of Canterbury.
//	Name: Unna Pussayapaiboon
//  Student ID: 64462335
//  FILE NAME: Cone.h
//  
//  Cone Class
//  ========================================================================

#ifndef H_CONE
#define H_CONE

#include "Object.h"

/**
 * Defines a simple Sphere located at 'center' 
 * with the specified radius
 */
class Cone : public Object
{

private:
    Vector center;
    float radius;
    float tall;

public:	
	Cone()
		: center(Vector()), radius(1)  //Default constructor creates a unit sphere
	{
		color = Color::WHITE;
	};
	
    Cone(Vector c, float r, float h, Color col)
		: center(c), radius(r), tall(h)
	{
		color = col;
	};
	bool checkTall(Vector pos, Vector dir, float t);
	
	float intersect(Vector pos, Vector dir);

	Vector normal(Vector p);

};

#endif //!H_CONE
