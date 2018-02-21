#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "bevgrafmath2017.h"

using namespace std;

GLsizei winWidth = 800, winHeight = 600;

struct Point
{
	double x, y;
};

GLint dragged = -1;
std::vector<vec2> controlPoints; // kontrollpontok

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(7.0);
	glLineWidth(2.0);
}

void drawBSpline(){
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		for(unsigned int i = 0; i<controlPoints.size(); i++)
			glVertex2d(controlPoints[i].x,controlPoints[i].y);
	glEnd();
	
	//kontrollPolygon kirajzolása
	glBegin(GL_LINE_STRIP);
		for(unsigned int i = 0; i<controlPoints.size(); i++)
			glVertex2d(controlPoints[i].x,controlPoints[i].y);
	glEnd();
	
	/*if (controlPoints.size() > 2)
	{
		TODO B-SPLINE
	}*/
}

GLint getActivePoint1(GLint sens, GLint x, GLint y) {
	GLint s = sens * sens;
	vec2 P = { (float)x, (float)y }; 

	for (unsigned int i=0; i< controlPoints.size();i++)
		if (dist(controlPoints[i], P) < s) // egérmutató és pont közötti távolságot és érzékenysége
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
		i = getActivePoint1(4, xMouse, winHeight - yMouse);
		if (i != -1) // elég közel van-e az egér, 8 sensitivity
			dragged = i;
		else{
			vec2 newPoint = {(float)xMouse, (float)(winHeight -yMouse)};
			controlPoints.push_back(newPoint);
		}
			
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
		dragged = -1;
	}
		
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse) { // pont húzása egérrel
	if (dragged >= 0) {
		controlPoints[dragged].x = xMouse;
		controlPoints[dragged].y = winHeight - yMouse;
	}
}

void display()
{	
	glClear(GL_COLOR_BUFFER_BIT);	
		
	drawBSpline();

	glutSwapBuffers();

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Zart B-Spline");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutMainLoop();

	return 0;
}