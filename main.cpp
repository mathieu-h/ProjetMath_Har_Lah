#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <numeric>

#include "Point.h"
#include "CPolygon.h"
#include "Window.h"
#include "CVector.h"

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


void DrawPolygon(CPolygon poly)
{
    glBegin(GL_POLYGON);
    glVertex2i(0,1);
    glVertex2i(-1,0);
    glVertex2i(1,0);
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
    
    CPolygon poly;
    std::vector<Point> points;
    Point p1(0,1);
    Point p2(-1,0);
    Point p3(1,0);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    
    poly.set_points(points);
    
    DrawPolygon(poly);
    
    glEnd();	//Pour les explications, lire le tutorial sur OGL et win
    glutSwapBuffers();
    
    //Attention : pas SwapBuffers(DC) !
    glutPostRedisplay();
}


void InitGL()
{
    
}

bool intersect(Point lastPointPoly, Point currentPointPoly, Point currentPointWindow, Point nextPointWindow)
{
    //TODO
    return false;
}

Point intersection(Point lastPointPoly, Point currentPointPoly, Point currentPointWindow, Point nextPointWindow)
{
    //TODO
    return Point();
}

/***  Determine if the point of the polygon is inside or outside the window   ***/
bool visible(Point lastPointPoly, Point currentPointWindow, Point nextPointWindow)
{
    //Vector in the same direction as the window's edge
    CVector vector_window(currentPointWindow, nextPointWindow);
    
    //Vector linking the window's edge and the polygon point
    CVector vector_poly(currentPointWindow, lastPointPoly);
    
    //We need the normal of the window edge vector
    CVector normal_window = vector_window.normal();
    
    //The dot product of the vector a . b = ax * bx + ay * by
    float dot_product = normal_window.diff_x() * vector_poly.diff_x() + normal_window.diff_y() * vector_poly.diff_y();
    
    return dot_product >= 0;
}

CPolygon windowing(const CPolygon polygon, const Window window)
{
    //TODO Algo Sutherland-Hodgman
    std::vector<Point> points_polygon = polygon.get_points();
    std::vector<Point> points_window = window.get_points();
    
    CPolygon polygonNew;
    
    for (std::size_t i = 1; i <= points_window.size(); ++i)
    {
        for (std::size_t j = 1; j <= points_polygon.size(); ++j)
        {
            if(j == 1)
            {
                
            }
            else
            {
                if(intersect(points_polygon[j-1], points_polygon[j], points_window[i], points_window[i+1]))
                {
                    Point intersectionPoint = intersection(points_polygon[j-1], points_polygon[j], points_window[i], points_window[i+1]);
                    polygonNew.addPoint(intersectionPoint);
                }
            }
            if(visible(points_polygon[j-1], points_window[i], points_window[i+1]))
            {
                polygonNew.addPoint(points_polygon[j-1]);
            }
        }
        if(polygonNew.get_points().size() > 0)
        {
            if(intersect(points_polygon[i], points_polygon[1], points_window[i], points_window[i+1]))
            {
                Point intersectionPoint = intersection(points_polygon[i-1], points_polygon[1], points_window[i], points_window[i+1]);
                polygonNew.addPoint(intersectionPoint);
            }
        }
        
        points_polygon = polygonNew.get_points();
        
    }
    
    
    return polygonNew;
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
    
    //glutMainLoop();
    
    Point p1(0,0);
    Point p2(1,0);
    Point p3(1,1);
    
    std::cout << visible(p3, p1, p2) << std::endl;
    
    p3.x_set(-1);
    p3.y_set(-1);
    
    std::cout << visible(p3, p1, p2)<< std::endl;
    
    return EXIT_SUCCESS;
}
