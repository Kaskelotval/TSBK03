
// Converted to MicroGlut and VectorUtils3 2013.
// MicroGLut currently exists for Linux and Mac OS X, and a beta for Windows.
// You will also need GLEW for Windows. You may sometimes need to work around
// differences, e.g. additions in MicroGlut that don't exist in FreeGlut.

// 2015: 

// Linux: gcc lab0.c ../common/*.c ../common/Linux/MicroGlut.c -lGL -o lab0 -I../common -I../common/Linux

// Mac: gcc lab0.c ../common/*.c ../common/Mac/MicroGlut.m -o lab0 -framework OpenGL -framework Cocoa -I../common/Mac -I../common

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	//uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
//#include "zpr.h"
#include "LoadTGA.h"

//constants
const int initWidth=512,initHeight=512;

// Model-to-world matrix
// Modify this matrix.
// See below for how it is applied to your model.
mat4 objectExampleMatrix = {{ 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0}};
// World-to-view matrix. Usually set by lookAt() or similar.
mat4 viewMatrix;
// Projection matrix, set by a call to perspective().
mat4 projectionMatrix;

//bunnymatrix
mat4 bunnyMatrix;
mat4 teddyMatrix;
mat4 teaMatrix;
// Globals
// * Model(s)
Model *bunny;
Model *teddy;
Model *tea;
// * Reference(s) to shader program(s)
GLuint program;
// * Texture(s)
GLuint texture;

void init(void)
{
	dumpInfo();

	// GL inits
	//glClearColor(0.2,0.2,0.5,0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	projectionMatrix = perspective(90, 1.0, 0.1, 1000);
	viewMatrix = lookAt(0,0,1.5, 0,0,0, 0,1,0);

	// Load and compile shader
	program = loadShaders("lab0.vert", "lab0.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:
	bunny = LoadModelPlus("objects/stanford-bunny.obj");
	printError("loaded bunny");
	teddy = LoadModelPlus("objects/teddy.obj");
	printError("loaded teddy");
	tea = LoadModelPlus("objects/teapot.obj");
	printError("loaded teapot");
	// Load textures
	LoadTGATextureSimple("textures/grass.tga",&texture);
	printError("load textures");

	bunnyMatrix = IdentityMatrix();
	teddyMatrix = IdentityMatrix();
	teaMatrix = IdentityMatrix();
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//textures
	glUniform1i(glGetUniformLocation(program,"exampletexture"),0);//the last argument has to be the same as the texture-unit that is to be used
	glActiveTexture(GL_TEXTURE0);//which texture-unit is active
	glBindTexture(GL_TEXTURE_2D, texture);//load the texture to active texture-unit
	//time
	GLfloat time = (GLfloat) glutGet(GLUT_ELAPSED_TIME);
	//activate the program, and set its variables
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	mat4 m = Mult(viewMatrix, objectExampleMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
	glUniform1f(glGetUniformLocation(program, "time"), time);
	
	//draw the bunny
	m = Mult(bunnyMatrix, T(-1.0,0.0,-1.0));	
	m = Mult(m,Ry(time*0.001));
	m = Mult(viewMatrix,m);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);	
	DrawModel(bunny, program, "in_Position", "in_Normal", "in_TexCoord");
	
	//draw the teddy
	m = Mult(bunnyMatrix, T(1.0,1.0,1.0));
	m = Mult(m,Ry(time*0.0005));	
	m = Mult(m, T(5.0,0.0,-1.0));	
	m = Mult(m,Rx(time*0.005));
	m = Mult(viewMatrix,m);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);	
	DrawModel(teddy, program, "in_Position", "in_Normal", "in_TexCoord");
	
	//draw the teapot
	m = Mult(teaMatrix, T(0.0,0.0,-2.0));
	m = Mult(m,Rz(time*0.0005));
	m = Mult(viewMatrix,m);
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"),1,GL_TRUE, m.m);
	DrawModel(tea,program, "in_Position", "in_Normal", "in_TexCoord");
	//Add a light source
	vec3 light1 ={90.0,0.0,0.0};
	glUniform3f(glGetUniformLocation(program,"lightPos"),light1.x,light1.y,light1.z);
	//Add a light source
	vec3 light2 ={10.0,10.0,10.0};
	light2 = MultVec3(Ry(time*0.002),light2); 
	glUniform3f(glGetUniformLocation(program,"light2Pos"),light2.x,light2.y,light2.z);
	//add something to show where light source is


	printError("display");
	
	glutSwapBuffers();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab 0 - OpenGL 3.2+ Introduction");
	glutDisplayFunc(display); 
	glutRepeatingTimer(20);
	init ();
	glutMainLoop();
	exit(0);
}

