#include <iostream>
#include <GL/glut.h>
#include <math.h>
#define M_PI 3.14159265358979323846
using namespace std;

int num_lados;
double radio;

void mostrar()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,1);
	glLoadIdentity();
	GLfloat grados_1;
	GLfloat grados_2;
	glBegin(GL_LINES);
	double puntos = 360.0f/num_lados;

    for(double i=0;i<360;i=i+puntos){
		grados_1 = (GLfloat)i*M_PI/180.0f;
		grados_2 = ((GLfloat)i+puntos)*M_PI/180.0f;
		glVertex3f(radio*cos(grados_1),radio*sin(grados_1),0.0f);
		glVertex3f(radio*cos(grados_2),radio*sin(grados_2),0.0f);
	}

	glEnd();
	glFlush();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void init()
{
	glClearColor(0,0,0,0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitmostrarMode(GLUT_SINGLE | GLUT_RGB);
	cout<<"Ingrese el numero de lados"<<endl;
	cin>>num_lados;
    cout<<"Ingrese radio:"<<endl;
	cin>>radio;
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Poligono");
	init();
	glutmostrarFunc(mostrar);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

