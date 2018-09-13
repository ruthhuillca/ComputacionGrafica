#include <stdio.h>
#include <stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>

using namespace std;

bool axis = false, flag = true;
int dibujo = 0;
GLfloat X = 2.0, Y = 2.0, Z = 1.0;

void dib_tortuga(void){
  glColor3f(1.0, 0.0, 0.0);
  double x[] = {0.0, -0.1, -0.15, -0.35, -0.45, -0.4, -0.3, -0.4, -0.35, -0.2,
  -0.35, -0.45, -0.35, -0.2, -0.15, -0.1, -0.1, -0.15, -0.2, -0.1, 0.0};
  double z[] = {-0.5, -0.55, -0.7, -0.75, -0.6, -0.45, -0.35, -0.2, 0.1, 0.25,
  0.3, 0.5, 0.6, 0.55, 0.45, 0.45, 0.6, 0.7, 0.8, 0.9, 0.95};
  glBegin(GL_LINE_LOOP);
  for(int i = 0; i < 21; i++){
    glVertex3f(x[i], 0.0, z[i]);
  }
  for(int i = 20; i >= 0; i--){
    glVertex3f(x[i] * -1.0, 0.0, z[i]);
  }
  glEnd();
}

void drawSphereTurtle(void){
  glColor3f(0.0, 0.0, 1.0);

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.0);
  glutWireSphere(0.4, 15, 15);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.55);
  glutWireSphere(0.2, 15, 15);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.35, 0.0, 0.35);
  glutWireSphere(0.1, 15, 15);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-0.35, 0.0, 0.35);
  glutWireSphere(0.1, 15, 15);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.35, 0.0, -0.35);
  glutWireSphere(0.1, 15, 15);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-0.35, 0.0, -0.35);
  glutWireSphere(0.1, 15, 15);
  glPopMatrix();
}


void ejes(void){

  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
  glVertex3f(5.0, 0.0, 0.0);
  glVertex3f(-5.0, 0.0, 0.0);
  glEnd();

  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 5.0, 0.0);
  glVertex3f(0.0, -5.0, 0.0);
  glEnd();

  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 5.0);
  glVertex3f(0.0, 0.0, -5.0);
  glEnd();
}

void display(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(dibujo == 0){
    dib_tortuga();
  }
  else {
    drawSphereTurtle();
  }
  if(axis){
    ejes();
  }
  glutSwapBuffers();
}

void reshape(int width, int height){
  if(width < height){
    glViewport(0, 0, width, width);
  }
  else {
    glViewport(0, 0, height, height);
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, 1.0, 0.0, 150.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
/// menu de las cosas que el ususario puede hacer con la tortuga
void keyboard(unsigned char key, int x, int y){
   switch (key){
    case 'h':
    printf("Ayuda:\n");
    printf("1. 'c' = Cambiar.\n");
    printf("2. 'q' or 'escape' = Salir.\n");
    printf("3. 'a' = Eje, (rojo = x, verde = y y azul = z).\n");
    printf("4. 'n' = Siguiente figura.\n");
    printf("5. '1' = Rotar en el eje x.\n");
    printf("6. '2' = Rotar en el eje y.\n");
    printf("7. '3' = Rotar en el eje z.\n");
    printf("8. 'v' = +z.\n");
    printf("9. 'b' = -z.\n");
    printf("10. 'd' = +y.\n");
    printf("11. 'u' = -y.\n");
    printf("12. 'r' = +x.\n");
    printf("13. 'l' = -x.\n");
    break;
    case 'a':
    /// Habilita o desabilita las ejes.
    axis = !axis;
    break;
    case 'c':
    /// Habilita o desabilita las lineas posteriores.
    if(glIsEnabled(GL_CULL_FACE)){
      glDisable(GL_CULL_FACE);
    }
    else {
      glEnable(GL_CULL_FACE);
    }
    break;
    /// Cambia el dibujo que se pintara.
    case 'n':
    if(dibujo == 1){
      dibujo = 0;
    }
    else {
      dibujo++;
    }
    break;
    case '1':
    /// Rota en el eje x.
    glRotatef(1.0, 1.0, 0.0, 0.0);
    break;
    case '2':
    /// Rota en el eje y.
    glRotatef(1.0, 0.0, 1.0, 0.0);
    break;
    case '3':
    /// Rota en el eje z.
    glRotatef(1.0, 0.0, 0.0, 1.0);
    break;
    case 'u':
	  Z += 0.1f;
    glLoadIdentity();
    cout << "x:" << X << "  y:" << Y << " z:" << Z << endl;
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	  break;
    case 'd':
	  Z -= 0.1f;
	  glLoadIdentity();
    cout << "x:" << X << "  y:" << Y << " z:" << Z << endl;
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	  break;
    case 'v':
	  Y -= 0.1f;
    glLoadIdentity();
    cout << "x:" << X << "  y:" << Y << " z:" << Z << endl;
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	  break;
    case 'b':
	  Y += 0.1f;
    glLoadIdentity();
    cout << "x:" << X << "  y:" << Y << " z:" << Z << endl;
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	  break;
    case 'l':
	  X -= 0.1f;
    glLoadIdentity();
    cout << "x:" << X << "  y:" << Y << " z:" << Z << endl;
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	  break;
    case 'r':
	  X += 0.1f;
    glLoadIdentity();
    cout << "x:" << X << "  y:" << Y << " z:" << Z << endl;
    gluLookAt(X, Y, Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	  break;
    case 'q':
    case 27:
    /// Salir
    exit(0);
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Tortuga");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
