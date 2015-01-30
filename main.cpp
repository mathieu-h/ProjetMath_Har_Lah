#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <numeric>

#include "Point.h"
#include "CPolygon.h"
#include "Window.h"
#include "CVector.h"
#include "Node.h"

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

#pragma region Windowing

float determinant(float matrix[2][2])
{
	return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
}


Point intersection(Point a, Point b, Point c, Point d)
{
	//matrice 1, matrice inverse.
	float matrixA[2][2];
	float matrixAReverse[2][2];
	float matrixRes[2];
	float matrixB[2];
	float det;

	float x1 = a.x_get();
	float x2 = b.x_get();
	float x3 = c.x_get();
	float x4 = d.x_get();

	float y1 = a.y_get();
	float y2 = b.y_get();
	float y3 = c.y_get();
	float y4 = d.y_get();

	Point p;

	matrixA[0][0] = (x2 - x1);
	matrixA[0][1] = (x3 - x4);
	matrixA[1][0] = (y2 - y1);
	matrixA[1][1] = (y3 - y4);

	matrixB[0] = (c.x_get() - a.x_get());
	matrixB[1] = (c.y_get()  - a.y_get() );

	det = determinant(matrixA);

	if(determinant == 0)
		throw 1;

	//Res = A-1 * B

	//The Matrix A must be reversed
	matrixAReverse[0][0] = matrixA[1][1] / det;
	matrixAReverse[0][1] = -matrixA[0][1] / det;
	matrixAReverse[1][0] = -matrixA[1][0] / det;
	matrixAReverse[1][1] = matrixA[0][0] / det;

	//Matrix product between reverse A and B matrix
	matrixRes[0] = matrixAReverse[0][0] * matrixB[0] + matrixAReverse[0][1] * matrixB[1];
	matrixRes[1] = matrixAReverse[1][0] * matrixB[0] + matrixAReverse[1][1] * matrixB[1];

	//The intersection is outise of the polygon current line segment
	if(matrixRes[0] > 1 || matrixRes[0] < 0)
		throw 2;

	//Calculate the intersection point
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
	points_window.push_back(points_window.front());

	CPolygon polygonNew;

	for (std::size_t i = 0; i < points_window.size() - 1; ++i)
	{
		polygonNew.clearPoints();
		for (std::size_t j = 0; j <= points_polygon.size()-1; ++j)
		{
			
			if(j > 0)
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

#pragma endregion

#pragma region Filling

std::vector<Node> AET;
std::vector<Node> ET;

void draw_pixel(int x,int y)
{
    glColor3f(0.0,1.0,1.0);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
}


#pragma endregion

#pragma region GLUT

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
			float new_x = convertViewportToOpenGLCoordinate(x/(float)width);

			float new_y = -convertViewportToOpenGLCoordinate(y/(float)height);

			Point p(new_x, new_y);
			polygon.addPoint(p);
			std::cout << p << std::endl;
		}
	}
}

void keyPressed(unsigned char key, int x, int y)
{
	if(key == 13)
	{
		CPolygon p = windowing(polygon, window);
		polygon = p;
	}else if(key == 'c')
	{
		polygon.clearPoints();
	}
}

void update()
{
	glutPostRedisplay();
}

void DrawPolygon(std::vector<Point> points)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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

#pragma endregion