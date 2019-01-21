//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury.
//	Name: Unna Pussayapaiboon
//  Student ID: 64462335
//  FILE NAME: surface.cpp
//  (modified from IcosahedronDraw.cpp in Ex-5)
//
//	The program loads and displays the mesh data for an icosahedron.
//  Renders only front facing triangles in wireframe mode.
//  The surface of the cone is then approximated
//
//  Requires files surface.h,  surface.vert, surface.cont, surface.eval, surface.geo, surface.frag
//  ========================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "surface.h"

using namespace std;

GLuint vaoID;
GLuint theProgram;
GLuint matrixLoc, matrixLoc2, matrixLoc3, lgtLoc;
float angle = 0.0, zoom = -18.0;
glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 30.0+zoom), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); //view matrix; 
glm::mat4 proj = glm::perspective(12.0f, 1.0f, 10.0f, 1000.0f);  //perspective projection matrix
glm::mat4 projView=proj*view;  //Matrices
GLuint tesslv;     //tessellation level factor
static float lv;
int wireframe_enable = 0;   //display mode

string readShaderFile(string filename) {
	ifstream shaderFile(filename.c_str());
	if (!shaderFile.good()) cout << "Error opening shader file. '" << filename << "'" << endl;
	stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();

	return shaderData.str();
}

GLuint loadShader(GLenum shaderType, string filename) {

	string shaderStr = readShaderFile(filename);
	const char* shaderTxt = shaderStr.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderTxt, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char *strShaderType = NULL;
		cerr <<  "Compile failure in shader: " << strInfoLog << endl;
		delete[] strInfoLog;
	}
	return shader;
}

void initialise() {
	GLuint shaderVert = loadShader(GL_VERTEX_SHADER, "surface.vert");
	GLuint shaderFrag = loadShader(GL_FRAGMENT_SHADER, "surface.frag");
	GLuint shaderTess = loadShader(GL_TESS_EVALUATION_SHADER, "surface.eval");
	GLuint shaderCont = loadShader(GL_TESS_CONTROL_SHADER, "surface.cont");
	GLuint shaderGeo = loadShader(GL_GEOMETRY_SHADER, "surface.geo");

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderVert);
	glAttachShader(program, shaderFrag);
	glAttachShader(program, shaderTess);
	glAttachShader(program, shaderCont);
	glAttachShader(program, shaderGeo);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	lv = 0;   //initialise value of tessellation level factor
	
	glUseProgram(program);
	matrixLoc = glGetUniformLocation(program, "mvpMatrix"); 	
	matrixLoc2 = glGetUniformLocation(program, "mvMatrix");   
	matrixLoc3 = glGetUniformLocation(program, "norMatrix");
	lgtLoc = glGetUniformLocation(program, "lightPos");
	tesslv = glGetUniformLocation(program, "tesslv");
	

	GLuint vboID[2];

	glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenVertexArrays(2, vboID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

    glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void update(int value) {
	angle += 0.5;
	glutTimerFunc(200, update, 0);
	glutPostRedisplay();
}

void display() {

	glm::mat4 matrix = glm::mat4(1.0);
	view = glm::lookAt(glm::vec3(0.0, 0.0, 30.0+zoom), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); //view matrix
	projView = proj*view;  //Product matrix
	
	matrix = glm::rotate(matrix, angle, glm::vec3(0.0, 1.0, 0.0));  //rotation matrix
	glm::mat4 prodMatrix2 = projView*matrix;        //Model-view-projn matrix


	//light part------------------------------------------
	glm::vec4 light = glm::vec4(20.0, 10.0, 20.0, 1.0);
	glm::mat4 prodMatrix1 = view*matrix;        //Model-view matrix
	glm::vec4 lightEye = view*light;     //Light position in eye coordinates
	glm::mat4 invMatrix = glm::inverse(prodMatrix1);  //Inverse of model-view matrix for normal transformation


	glUniformMatrix4fv(matrixLoc2, 1, GL_FALSE, &prodMatrix1[0][0]);   //mvmatrix
	glUniformMatrix4fv(matrixLoc3, 1, GL_TRUE, &invMatrix[0][0]);  //Use transpose matrix here
	glUniform4fv(lgtLoc, 1, &lightEye[0]);

	//----------------------------------------------------------------------

	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &prodMatrix2[0][0]);
	glUniform1f(tesslv, lv);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vaoID);

	glDrawElements(GL_PATCHES ,60, GL_UNSIGNED_SHORT, NULL);

	
	glPatchParameteri(GL_PATCH_VERTICES, 3);


	
	glFlush();
}

//Special keyboard function
void special(int key, int x, int y) {
	if(key == GLUT_KEY_DOWN && lv>-3){
		 //zoom += 2;
		 lv--;
	 }
	else if(key == GLUT_KEY_UP && lv<9){
		 //zoom -= 2;
		 lv++;
	 }
	 
	 if (key == GLUT_KEY_LEFT){
		 if (wireframe_enable == 0){
			 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			 wireframe_enable = 1;
		 }
		 else{
			 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			 wireframe_enable = 0;
		 }
	 }
	 
	 
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Surface");
	glutInitContextVersion (4, 2);
	glutInitContextProfile ( GLUT_CORE_PROFILE );

	if(glewInit() == GLEW_OK)
	{
		cout << "GLEW initialization successful! " << endl;
		cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;
	}
	else
	{
		cerr << "Unable to initialize GLEW  ...exiting." << endl;
		exit(EXIT_FAILURE);
	}

	initialise();
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutTimerFunc(50, update, 0); 
	glutMainLoop();
}

