#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Point.h"
#include "Segment.h"
#include "Polygon.h"
#include "Window.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

int WindowName;


void Reshape(int width, int height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
                   45,
                   float(width)/float(height),
                   0.1,
                   100
                   );	//Pour les explications, lire le tutorial sur OGL et win
    glMatrixMode(GL_MODELVIEW);	//Optionnel
}

void Draw()
{	glClear
    (
     GL_COLOR_BUFFER_BIT |
     GL_DEPTH_BUFFER_BIT
     );	//Efface le frame buffer et le Z-buffer
    glMatrixMode(GL_MODELVIEW);	//Choisit la matrice MODELVIEW
    glLoadIdentity();	//Réinitialise la matrice
    gluLookAt(0,0,-10,0,0,0,0,1,0);
    glBegin(GL_TRIANGLES);
    glVertex2i(0,1);
    glVertex2i(-1,0);
    glVertex2i(1,0);
    
    glEnd();	//Pour les explications, lire le tutorial sur OGL et win
    glutSwapBuffers();
    
    //Attention : pas SwapBuffers(DC) !
    glutPostRedisplay();
}

void InitGL()
{
    
}

bool intersect(Segment seg_polygon, Segment seg_window)
{
    //TODO
    return false;
}

Point intersection(Segment seg_polygon, Segment seg_window)
{
    //TODO
    return *new Point();
}

bool visible(Point p, Segment seg_window)
{
    //TODO
    return false;
}

Polygon windowing(const Polygon polygon, const Window window)
{
    //TODO Algo Sutherland-Hodgman
    return *new Polygon();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640,480);	//Optionnel
    WindowName = glutCreateWindow("Ma première fenêtre OpenGL !");
    //glutFullScreen();	//Optionnel
    
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Draw);
    InitGL();
    
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
