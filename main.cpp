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

float determinant(float w, float x, float y, float z)
{
    return (w * z) - (y * x);
}

bool intersect(Point lastPointPoly, Point currentPointPoly, Point currentPointWindow, Point nextPointWindow)
{
    float x1 = lastPointPoly.x_get();
    float x2 = currentPointPoly.x_get();
    float x3 = currentPointWindow.x_get();
    float x4 = nextPointWindow.x_get();
    
    float y1 = lastPointPoly.y_get();
    float y2 = currentPointPoly.y_get();
    float y3 = currentPointWindow.y_get();
    float y4 = nextPointWindow.y_get();
    
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if(den == 0)
    {
        return false;
    }
    
    return true;
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
    
    float px = (determinant(determinant(x1, y1, x2, y2), determinant(x1, 1, x2, 1), determinant(x3, y3, x4, y4), determinant(x3, 1, x4, 1)))/(determinant(determinant(x1, 1, x2, 1), determinant(y1, 1, y2, 1), determinant(x3, 1, x4, 1), determinant(y3, 1, y4, 1)));
    
    float py = (determinant(determinant(x1, y1, x2, y2), determinant(y1, 1, y2, 1), determinant(x3, y3, x4, y4), determinant(y3, 1, y4, 1)))/(determinant(determinant(x1, 1, x2, 1), determinant(y1, 1, y2, 1), determinant(x3, 1, x4, 1), determinant(y3, 1, y4, 1)));
    
    return Point(px, py);
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
    if(key == '\n')
    {
        std::cout << "ENTER pressed" << std::endl;
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
    
    std::vector<Point> window_points;
    window_points.push_back(Point(-0.5, 0.5));
    window_points.push_back(Point(0.5, 0.5));
    window_points.push_back(Point(0.5, -0.5));
    window_points.push_back(Point(-0.5, -0.5));
    
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
