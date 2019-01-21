//  ========================================================================
//  COSC363: Computer Graphics (2013);  University of Canterbury.
//	Name: Unna Pussayapaiboon
//  Student ID: 64462335
//  FILE NAME: RayTracer.cpp
//  
//	Assignment 2
//  ========================================================================
#include <iostream>
#include <cmath>
#include <vector>
#include "Vector.h"
#include "Sphere.h"
#include "Color.h"
#include "Object.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Cone.h"
#include <GL/glut.h>

//=================== GLOBAL VARIABLES ======================

using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int PPU = 30;                 //Total 600x600 pixels
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<Object*> sceneObjects;

Vector light = Vector(10.0, 40.0, -5.0);
Color backgroundCol = Color::GRAY;

//Bump Mapping
int current_bump=0;
float noiseCofx=0.1, noiseCofy=0.1, noiseCofz=1.5;


//=============================================================================

struct PointBundle   
{
	Vector point;
	int index;
	float dist;
};

/*
* This function compares the given ray with all objects in the scene
* and computes the closest point  of intersection.
*/
PointBundle closestPt (Vector pos, Vector dir) {
    Vector point(0, 0, 0);
	float min = 10000.0;

	PointBundle out = {point, -1, 0.0};

    for (unsigned int i = 0;  i < sceneObjects.size();  i++)
	{
        float t = sceneObjects[i]->intersect(pos, dir);
		if (t > 0) {
			point = pos + dir*t;
			if (t < min) {
				out.point = point;
				out.index = i;
				out.dist = t;
				min = t;
			}
		}
	}

	return out;
}


/*
* Computes the colour value obtained by tracing a ray.
* If reflections and refractions are to be included, then secondary rays will 
* have to be traced from the point, by converting this method to a recursive
* procedure.
*/
Color trace (Vector pos, Vector dir, int step) {
	
    PointBundle q = closestPt(pos, dir);
    if(q.index == -1) return backgroundCol;        //no intersection

	Vector n = sceneObjects[q.index]->normal(q.point);	
	Vector l = light - q.point;
	l.normalise();
	float lDotn = l.dot(n);

	Color col = sceneObjects[q.index]->getColor();

//-------------------------------3D TEXTURED SPHERE - Bumpy Surface
	if (q.index == 5) {
		if (current_bump < 2) current_bump++;
		else if (current_bump) current_bump--;
		n.x += n.x+current_bump * noiseCofx;
		n.y += n.y+current_bump * noiseCofx;
		n.z += n.z+current_bump * noiseCofz;
		
	}

//-------------------------------TEXTURED SPHERE------------------
	if (q.index == 1) {
			if( ( q.point.z > 0 && int (q.point.z) % 2 < 1) || int (-q.point.z) % 2 >= 1 )
			col = sceneObjects[q.index]->getColor();
			else col = Color(0.7, 0.93, 0.23);
	}
	
//------------------------------TEXTURED PLANE----------------    
	else if (q.index == 3) {
		if( ( q.point.z > 0 && int (q.point.z) % 10 < 5) || int (-q.point.z) % 10 >= 5 ) {
			if( (q.point.x > 0 && int (q.point.x)%10 < 5) || int (-q.point.x) % 10 >= 5 )
				col = Color(0.93, 0.47, 0.26);
			}
		else {
			if( ( q.point.x > 0 && int (q.point.x)%10 < 5) || int (-q.point.x) % 10 >= 5 )
			col = sceneObjects[q.index]->getColor();
			else col = Color(0.93, 0.47, 0.26);
		}
	}
//------------------------------CYLINDER---------------------
	else if (q.index == 4) {
		if( ( (q.point.y > 0 && int (q.point.y) % 2 < 1) || int (-q.point.y) % 2 >= 1  ) ) {
			col.combineColor (Color(0.8, 0.57, 0.62));
		}
	}
//------------------------------TEXTURED CUBE----------------
	else if (q.index >= 6 && q.index <= 11) {
		if ( ( ( q.point.y > 0 && int (q.point.y) % 2 < 1) || int (-q.point.y) % 2 >= 1  ) ) {
			if( (q.point.x > 0 && int (q.point.x) % 2 < 1) || int (-q.point.x) % 2 >= 1 )
				col.combineColor (Color(0.7, 0.8, 0.7));

			
			if( (q.point.z > 0 && int(q.point.z) % 2 < 1) || int (-q.point.z) % 2 >= 1 )
				col.combineColor (Color(0.7, 0.8, 0.7));
		}
		else {
			if( (q.point.y > 0 && int(q.point.y) % 2 < 1) || int (-q.point.y) % 2 >= 1 )
			col = sceneObjects[q.index]->getColor();
			else col.combineColor (Color(0.7, 0.8, 0.7));
		}
	}
//--------------------------------------------------------------
	
    PointBundle q2 = closestPt (q.point, l);  //for shadow

	float reflCoeff = 0.6;
	Vector r = ((n * 2) * lDotn) - l;   // r = 2(L.n)n – L.
	r.normalise();
	Color colorSum;
	
	Vector v(-dir.x, -dir.y, -dir.z);   //View vector;
	float rDotv = r.dot(v);


	//In Shadow
	if (lDotn <= 0) colorSum = col.phongLight (backgroundCol, 0.0, 0.0);
	
	//In Light
	else{
		
		if (q2.index != -1) colorSum = col.phongLight (backgroundCol, 0.0, 0.0);
		else{ 
			float spec;
			if(rDotv < 0) spec = 0.0;
			else spec = pow(rDotv, 10);         //Phong exponent = 10
			colorSum = col.phongLight (backgroundCol, lDotn, spec);
		}
		
	}
	
	//Reflection---------------------------------------------------
	if (q.index == 0 && step < MAX_STEPS)
	{
		r = (n * 2) * v.dot(n) - v;      // r = 2(v.n)n – v.
		Color reflectionCol = trace(q.point, r, step + 1);
		colorSum.combineColor(reflectionCol, reflCoeff);
	}
	else if ((q.index == 3 || q.index == 2) && step < MAX_STEPS) { // Gloss Plane
		r = (n * 2) * v.dot(n) - v;      // r = 2(v.n)n – v.
		Color reflectionCol = trace(q.point, r, step + 1);
		colorSum.combineColor(reflectionCol, 0.2);
	}
	
	//Refraction---------------------------------------------------
	if(q.index == 2 && step < MAX_STEPS)
	{
		float dDotn = dir.dot(n);
		float m_ratio = 1 / 1.001;    // air/glass
		
		if (dDotn < 0) {
			float cos_delta_t = sqrt( 1 - ((m_ratio * m_ratio) * (1 - (dDotn * dDotn))) );
			Vector g = dir * (m_ratio) - n * ((m_ratio) * dDotn + cos_delta_t);
			
			Color refractCol = trace(q.point, g, step+1);
			
			colorSum.combineColor(refractCol, 0.8);
		}
		if (dDotn >= 0) {
			m_ratio = 1.001;     // glass/air
			n *= -1;
			
			dDotn = dir.dot(n);
			float cos_delta_t = sqrt( 1 - ((m_ratio * m_ratio) * (1 - (dDotn * dDotn))) );
			Vector g = dir * (m_ratio) - n * ((m_ratio) * dDotn + cos_delta_t);
			
			Color refractCol = trace(q.point, g, step + 1);
			colorSum.combineColor(refractCol, 0.8);
		} 	
	}	
	return colorSum;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each pixel as quads.
//---------------------------------------------------------------------------------------
void display() {
	int widthInPixels = (int)(WIDTH * PPU);
	int heightInPixels = (int)(HEIGHT * PPU);
	float pixelSize = 1.0 / PPU;
	float halfPixelSize = pixelSize / 2.0;
	float x1, y1, xc, yc;
	Vector eye (0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);  //Each pixel is a quad.

	for (int i = 0; i < widthInPixels; i++) {
		x1 = XMIN + i*pixelSize;
		xc = x1 + halfPixelSize;
		for (int j = 0; j < heightInPixels; j++) {
			y1 = YMIN + j*pixelSize;
			yc = y1 + halfPixelSize;

		    Vector dir(xc, yc, -EDIST);	//direction of the primary ray
		    dir.normalise();			//Normalise this direction

		    Color col = trace (eye, dir, 1); //Trace the primary ray and get the colour value

			glColor3f (col.r, col.g, col.b);
			
			
			glVertex2f (x1, y1);				//Draw each pixel with its color value
			glVertex2f (x1 + pixelSize, y1);
			glVertex2f (x1 + pixelSize, y1 + pixelSize);
			glVertex2f (x1, y1 + pixelSize);
        }
    }

    glEnd();
    glFlush();
}


/* Function for drawing a cube by creating 6 planes */
void draw_cube (Vector A, Vector B, Vector C, Vector D, Vector E, Vector F, Vector G, Vector H, Color col) {
	
	//top
	Plane *cube1 = new Plane (D, C, E, F, col);  
	sceneObjects.push_back (cube1);
	
	//front
    Plane *cube2 = new Plane (A, B, C, D, col);  
	sceneObjects.push_back (cube2);	
	//right
	Plane *cube3 = new Plane (B, H, E, C, col);  
	sceneObjects.push_back (cube3);	
	
	//back
	Plane *cube4 = new Plane (H, G, F, E, col);  
	sceneObjects.push_back (cube4);	

	//left
	Plane *cube5 = new Plane (G, A, D, F, col);  
	sceneObjects.push_back (cube5);
	
	//buttom
	Plane *cube6 = new Plane (G, H, B, A, col);  
	sceneObjects.push_back (cube6);			
		
}

void initialize () {
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (XMIN, XMAX, YMIN, YMAX);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glClearColor (0, 0, 0, 1);
    
    //index=0
    Sphere *sphere1 = new Sphere (Vector(2, 5, -75), 12.0, Color(0,0.5,0.5));   //reflection sphere
    sceneObjects.push_back(sphere1);
    
    //index=1
	Sphere *sphere2 = new Sphere (Vector(-8, -2, -55), 3.5, Color(0.19, 0.5, 0.078));  //green sphere
    sceneObjects.push_back(sphere2);
    
    //index=2
	Sphere *sphere3 = new Sphere (Vector(2, 0, -20), 2, Color(0.1, 0.1, 0.1));     //refraction sphere
    sceneObjects.push_back(sphere3);  

	//index=3
    Plane *plane = new Plane (Vector(-17, -10, -40), Vector(17, -10, -40),
	Vector(17, -10, -100), Vector(-17, -10, -100), Color(0.7, 0.7, 0.7));  
	sceneObjects.push_back(plane);
	
	//index=4
	Cylinder *cylinder = new Cylinder (Vector(-3, -10, -45), 2, 7, Color(1, 0.84, 0));
    sceneObjects.push_back(cylinder);  


	//index=5
	Sphere *sphere4 = new Sphere (Vector(-4, 5, -40), 3.5, Color(1, 0, 0));         //Bumpy Sphere
    sceneObjects.push_back(sphere4);  

	//index=6-11
	draw_cube (Vector(5, -8, -50), Vector(10, -8, -50), 
	Vector(10, -3, -50), Vector(5, -3, -50),
	
	Vector(10, -3, -55), Vector(5, -3, -55),
	Vector(5, -8, -55), Vector(10, -8, -55),
	
	Color(1, 0.51, 0.67));
	
	//index=12
	Cone *cone = new Cone (Vector(2, -10, -50), 1, 5, Color(0.7, 0.32, 0.8));
    sceneObjects.push_back(cone); 
}


int main (int argc, char *argv[])  {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (20, 20);
    glutCreateWindow ("Raytracing");

    glutDisplayFunc (display);
    initialize ();

    glutMainLoop ();
    return 0;
}
