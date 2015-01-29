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
#endif

int height = 640;
int width = 640;

CPolygon polygon;
Window window;

float determinant(float matrix[2][2])
{
    return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
}

bool intersect(Point lastPointPoly, Point currentPointPoly, Point currentPointWindow, Point nextPointWindow)
{
    //A refaire
    float x1 = lastPointPoly.x_get();
    float x2 = currentPointPoly.x_get();
    float x3 = currentPointWindow.x_get();
    float x4 = nextPointWindow.x_get();
    
    float y1 = lastPointPoly.y_get();
    float y2 = currentPointPoly.y_get();
    float y3 = currentPointWindow.y_get();
    float y4 = nextPointWindow.y_get();
    
    float den = (x2 - x1) * (y3 - y4) - (y2 - y1) * (x3 - x4);
    if(den == 0)
    {
        //Line are parallel
        std::cout << "parallel" << std::endl;
        return false;
    }
    
    float t = -(x1*(y4-y3)-x3*(y4-y3)-(x4-x3)*y1+(x4-x3)*y3)/den;
    float s = -(-(x2-x1)*y1 + (x2-x1)*y3 - (y2-y1)*x3)/den;
    
    //The intersection is on the line segment
    if(/*s >= 0 && s <= 1 &&*/ t >= 0 && s <= 1)
    {
        std::cout << "Intersection on the line segment" << std::endl;
        return true;
    }
    //The intersection is outside of the line segment
    else
    {
        std::cout << "Intersection outside of the line segment" << std::endl;
        return false;
    }
}

Point intersection(Point lastPointPoly, Point currentPointPoly, Point currentPointWindow, Point nextPointWindow)
{
    float x1 = lastPointPoly.x_get();
    float x2 = currentPointPoly.x_get();
    float x3 = currentPointWindow.x_get();
    float x4 = nextPointWindow.x_get();
    
    float y1 = lastPointPoly.y_get();
    float y2 = currentPointPoly.y_get();
    float y3 = currentPointWindow.y_get();
    float y4 = nextPointWindow.y_get();
    
    float matrixA[2][2];
    float matrixAReverse[2][2];
    float matrixRes[2];
    float matrixB[2];
    float det;
    
    Point p;
    
    matrixA[0][0] = (x2 - x1);
    matrixA[0][1] = (x3 - x4);
    matrixA[1][0] = (y2 - y1);
    matrixA[1][1] = (y3 - y4);
    
    det = determinant(matrixA);
    
    if(det == 0){
        std::cout << "parallel" << std::endl;
        throw 1;
    }
    
    matrixAReverse[0][0] = matrixA[1][1] / det;
    matrixAReverse[0][1] = matrixA[1][0] / det;
    matrixAReverse[1][0] = matrixA[0][1] / det;
    matrixAReverse[1][1] = matrixA[0][0] / det;
    
    matrixB[0] = (x3 - x1);
    matrixB[1] = (y3 - y1);
    
    //Matrix product
    matrixRes[0] = matrixAReverse[0][0] * matrixB[0] + matrixAReverse[0][1] * matrixB[1];
    matrixRes[1] = matrixAReverse[1][0] * matrixB[0] + matrixAReverse[1][1] * matrixB[1];
    
    if(matrixRes[0] < 0 || matrixRes[0] > 1)
    {
        std::cout << "Intersection outside of the line segment" << std::endl;
        throw 2;
    }
    std::cout << "Intersection on the line segment" << std::endl;
    
    p.x_set(((1 - matrixRes[0]) * x1) + (matrixRes[0] * x2));
    p.y_set(((1 - matrixRes[0]) * y1) + (matrixRes[0] * y2));
    
    
    return p;
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
    std::vector<Point> points_polygon = polygon.get_points();
    std::vector<Point> points_window = window.get_points();
    
    CPolygon polygonNew;
    
    for (std::size_t i = 0; i < points_window.size(); ++i)
    {
        polygonNew.clearPoints();
        for (std::size_t j = 0; j <= points_polygon.size()-1; ++j)
        {
            if(j == 0)
            {
                
            }
            else
            {
                try {
                    Point intersectionPoint = intersection(points_polygon[j-1], points_polygon[j], points_window[i], points_window[i+1]);
                    polygonNew.addPoint(intersectionPoint);
                }
                catch(int e)
                {
                    
                }
            }
            if(visible(points_polygon[j], points_window[i], points_window[i+1]))
            {
                polygonNew.addPoint(points_polygon[j]);
            }
        }
        if(polygonNew.get_points().size() > 0)
        {
            try {
                Point intersectionPoint = intersection(points_polygon[points_polygon.size()-1], points_polygon[0], points_window[i], points_window[i+1]);
                polygonNew.addPoint(intersectionPoint);
            }
            catch(int e)
            {
                
            }
        }
        
        points_polygon = polygonNew.get_points();
        
    }
    
    polygonNew.set_points(points_polygon);
    return polygonNew;
}

float convertViewportToOpenGLCoordinate(float x)
{
    return (x * 2) - 1;
}

void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            std::cout << "x :" << x << std::endl;
            float new_x = convertViewportToOpenGLCoordinate(x/(float)width);
            std::cout << "new x :" << new_x << std::endl;
            
            std::cout << "y :" << y << std::endl;
            float new_y = -convertViewportToOpenGLCoordinate(y/(float)height);
            std::cout << "new y :" << new_y << std::endl;
            
            Point p(new_x, new_y);
            polygon.addPoint(p);
            std::cout << p << std::endl;
        }
    }
}

void keyPressed(unsigned char key, int x, int y)
{
    std::cout << " " << key << std::endl;
    if(key == 13)
    {
        std::cout << "ENTER pressed" << std::endl;
        CPolygon p = windowing(polygon, window);
        polygon = p;
        std::cout << p << std::endl;
    }
}

void update()
{
    glutPostRedisplay();
}

void DrawPolygon(std::vector<Point> points)
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3d((float)(127.f/255.f), (float)(48.f/255.f), (float)(201.f/255.f));
    glBegin(GL_POLYGON);
    
    for (std::size_t i = 0; i < points.size()  ; ++i) {
        glVertex3f(points[i].x_get(), points[i].y_get(), 0.0);
    }
    glEnd();
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glColor3d(0, 0, 0);
    glBegin(GL_POLYGON);
    
    glVertex3f(-0.5, 0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    
    glEnd();
    
    
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
    
    
    //    DrawPolygon(window.get_points());
    DrawPolygon(polygon.get_points());
    
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    
    Point p1(-0.5, 0.5);
    Point p2(0.5, 0.5);
    Point p3(0.5, -0.5);
    Point p4(-0.5, -0.5);
    Point p5(0.5, 1);
    Point p6(1, 0.5);
    Point p7(0.5, 0);
    
    
    try
    {
        //Inside
        std::cout << intersection(p1, p2, p5, p3) << std::endl << std::endl;
        //Inside
        std::cout << intersection(p1, p6, p5, p3) << std::endl << std::endl;
        
        //Parallel
        std::cout << intersection(p1, p2, p4, p3) << std::endl << std::endl;
        
    }
    catch(int e)
    {
        try {
            //Outside
            std::cout << intersection(p7, p3, p1, p6) << std::endl << std::endl;
        } catch (int e) {
            
        }
    }
    
    
    window.add_point(p1);
    window.add_point(p2);
    window.add_point(p3);
    window.add_point(p4);
    
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(width,height);
    glutCreateWindow("Lighthouse3D - GLUT Tutorial");
    
    // register callbacks
    glutDisplayFunc(renderScene);
    glutMouseFunc (MouseButton);
    glutIdleFunc(update);
    glutKeyboardFunc(keyPressed);
    
    // enter GLUT event processing cycle
    glutMainLoop();
    
    return 1;
}
