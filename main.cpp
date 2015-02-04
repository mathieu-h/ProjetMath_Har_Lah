#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <limits>

#include "Point.h"
#include "CPolygon.h"
#include "Window.h"
#include "CVector.h"
#include "Edge.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

float height = 640.0f;
float width = 640.0f;

CPolygon polygon;
Window window;
typedef Edge* EdgePtr;

#pragma mark Windowing
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
    
    if(det == 0)
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


#pragma mark Filling
#pragma region Filling

//Active Edge table
std::vector<Edge> AET;

//Edge tabel
std::vector<Edge> ET;


void draw_line(Point& a, Point& b)
{
    glColor3d(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(a.x_get(),a.y_get());
    glVertex2f(b.x_get(),b.y_get());
    glEnd();
}

float calculateSlope(Point a, Point b)
{
    return (b.y_get() - a.y_get())/(b.x_get() - a.x_get());
}


float convertOpenGLToViewportCoordinate(float x)
{
    return (x + 1)/2;
}


void InsertIntoEdgeTable(std::vector<Edge>& edgeTable, Edge& e, int index)
{
    if(edgeTable[index].isEmpty())
    {
        e.setNext(0);
        edgeTable[index] = e;
    }
    else
    {
        EdgePtr currentNode = &edgeTable[index];
        while(currentNode->getNext() != 0)
        {
            currentNode = currentNode->getNext();
        }
        e.setNext(0);
        currentNode->setNext(&e);
    }
}

void createEdgeTable(CPolygon const &polygon)
{
	Edge emptyEdge;
    //std::vector<Edge> newET(glutGet(GLUT_WINDOW_HEIGHT),emptyEdge);
	
	for(std::size_t i = 0 ; i < glutGet(GLUT_WINDOW_HEIGHT) ; i++)
    {
        Edge e;
        ET.insert(ET.begin() + i, e);
	}
    
	
    std::vector<Point> points = polygon.get_points();
    
    for (std::size_t i = 1; i <= points.size(); ++i)
    {
        Point start = points[i-1];
        Point end;
        if(i == points.size())
        {
            end = points[0];
        }
        else
        {
            end = points[i];
        }
        
        float slope = calculateSlope(start, end);
        if(slope == 0)
        {
            continue;
        }
        float yMax = (start.y_get() > end.y_get())?start.y_get():end.y_get();
        float yMin = (start.y_get() < end.y_get())?start.y_get():end.y_get();
        float xMin = (start.x_get() < end.x_get())?start.x_get():end.x_get();
        
        Edge edge(yMax, yMin, xMin, 1.0f/slope);
		//edge.getXMin();

        float index = convertOpenGLToViewportCoordinate(yMin);
        index *= glutGet(GLUT_WINDOW_HEIGHT);
        int indexInt = (int) index;
        
        InsertIntoEdgeTable(ET, edge, indexInt);
    }
    
    for (int i = 0; i < ET.size(); i++) {
		if(!ET[i].isEmpty()){
			std::cout << i << " : " << ET[i] << std::endl;
		}
    }
}


EdgePtr InsertNodesIntoLCA(EdgePtr ptrLCA, const std::vector<Edge>& vectorSI, int i){
	EdgePtr currentNode = ptrLCA;
	if(!vectorSI[i].isEmpty()){
		// add while vectorSI[i]->getNext() pour bien ajouter tous les noeuds à cette valeur de y dans la LCA
		EdgePtr edgeLCA = new Edge(vectorSI[i]);
		edgeLCA->setNext(0);
		if(ptrLCA == 0){			
			ptrLCA = edgeLCA;
		}else{
			while(currentNode->getNext() != 0){ 
				currentNode = currentNode->getNext();
			}
			currentNode->setNext(edgeLCA);
		}
	}
	return ptrLCA;
}

EdgePtr RemoveNodesFromLCA(EdgePtr ptrLCA, int i){
	EdgePtr currentNode = ptrLCA;
	if(ptrLCA == 0){
		return ptrLCA;
	}
	// ajouter la conversion Y coordonnée viewport et openGL (les indices sont des entiers)
	if(currentNode->getYMax() == i){
		if(currentNode->getNext() == 0){		
			currentNode = 0;
			return currentNode;
		}else{
			EdgePtr oldCurrentNode = currentNode;
			currentNode = oldCurrentNode->getNext();
			delete oldCurrentNode;
			oldCurrentNode = 0;
			return currentNode;
		}
	}else{
		currentNode->setNext(RemoveNodesFromLCA(currentNode->getNext(), i));
		return currentNode;
	}
}

EdgePtr SortLCA(EdgePtr list,int (*compare)(EdgePtr one,EdgePtr two))
{
    // Trivial case.
    if (!list || !list->getNext())
        return list;

    EdgePtr right = list,
          temp  = list,
          last  = list,
          result = 0,
          next   = 0;

    // Find halfway through the list (by running two pointers, one at twice the speed of the other).
    while (temp && temp->getNext())
    {
        last = right;
        right = right->getNext();
        temp = temp->getNext()->getNext();
    }

    // Break the list in two. (prev pointers are broken here, but we fix later)
    last->setNext(0);

    // Recurse on the two smaller lists:
    list = SortLCA(list, compare);
    right = SortLCA(right, compare);

    // Merge:
    while (list || right)
    {
        // Take from empty lists, or compare:
        if (!right) {
            next = list;
            list = list->getNext();
        } else if (!list) {
            next = right;
            right = right->getNext();
        } else if (compare(list, right) < 0) {
            next = list;
            list = list->getNext();
        } else {
            next = right;
            right = right->getNext();
        }		
        if (!result) {
            result=next;
        }
    }
    return result;
}

int compare(const EdgePtr one, const EdgePtr two){
	int firstX = one->getXMin();
	int secondX = one->getXMin();
	if(firstX < secondX){
		return -1;
	}
	else if( firstX == secondX){
		return 0;
	}
	else{
		return 1;
	}
}

void FillingLCALoop(CPolygon const &polygon){
	createEdgeTable(polygon);
	EdgePtr ptrLCA = 0;
    int i = 0;
    while(ET[i].isEmpty()){
        i++;
    }
    EdgePtr edge = ET[i].getNext();
    std::cout << "GET NEXT : " << *edge << std::endl;
	for(int i = 0 ; i < ET.size() ; i++){
		// vérifier si la ptrLCA est bien changée, à voir si il ne faut pas que Insert la renvoie ou qu'on passe la prtLCA par valeur 
		// et non par copie
		/*
		if(vectorSI[i].isEmpty()){
			printf("Hello");
		}*/
		// TODO
		ptrLCA = InsertNodesIntoLCA(ptrLCA, ET, i);
		ptrLCA = RemoveNodesFromLCA(ptrLCA, i);
		//ptrLCA = SortLCA(ptrLCA, &compare);
		//DisplaySegments();
	}

}


#pragma endregion

#pragma mark GLUT
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
            //std::cout << x << " " << y << std::endl;
			float new_x = convertViewportToOpenGLCoordinate(x/(float)width);

			float new_y = -convertViewportToOpenGLCoordinate(y/(float)height);

			Point p(new_x, new_y);
			polygon.addPoint(p);
			//std::cout << p << std::endl;
		}
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            float index = convertViewportToOpenGLCoordinate(y/(float)height);
            index = convertOpenGLToViewportCoordinate(index);
            index *= glutGet(GLUT_WINDOW_HEIGHT);

            std::cout << y << std::endl;
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
	}else if(key == 'f')
    {
        FillingLCALoop(polygon);
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
    
    if(points.size() <= 2){
        
        glBegin(GL_LINES);
        for (std::size_t i = 0; i < points.size()  ; ++i) {
            glVertex2f(points[i].x_get(), points[i].y_get());
        }
        glEnd();
    }
    else
    {
        glBegin(GL_POLYGON);

        for (std::size_t i = 0; i < points.size()  ; ++i) {
            glVertex2f(points[i].x_get(), points[i].y_get());
        }
        glEnd();
    }

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);

	glVertex3f(-0.5, 0.5, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	glVertex3f(0.5, -0.5, 0.0);
	glVertex3f(-0.5, -0.5, 0.0);

	glEnd();

    /*float y = -1;
    
    Point a;
    Point b;
    a.x_set(-0.5);
    b.x_set(0.5);
    
    for (int i = 0; i < glutGet(GLUT_WINDOW_HEIGHT); ++i)
    {
        draw_line(a, b);
        y+= 2.0f/glutGet(GLUT_WINDOW_HEIGHT);
        a.y_set(y);
        b.y_set(y);
    }*/

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
	glutCreateWindow("Projet Math - GLUT");

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