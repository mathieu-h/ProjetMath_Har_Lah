//
//  Polygon.h
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#ifndef __Cours1__Polygon__
#define __Cours1__Polygon__
#include "Point.h"
#include <vector>

class Polygon
{
private:
    std::string name;
    std::vector<Point> points;
    
public:
    
    /* Contructor and Desctructor */
    Polygon();
    Polygon(const std::vector<Point> &points, std::string name);
    Polygon(const Polygon &poly);
    ~Polygon(void);
    
    /* Getter and Setter */
    std::string get_name() const;
    std::vector<Point> get_points() const;
    
    void set_name(std::string name);
    void set_points(std::vector<Point> points);
    
    void addPoint(Point point);
    
};

#endif /* defined(__Cours1__Polygon__) */
