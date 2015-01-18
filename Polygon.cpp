//
//  Polygon.cpp
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#include "Polygon.h"

Polygon::Polygon()
{
    this->name = "Mon Polygone";
}

Polygon::Polygon(const std::vector<Point> &points, std::string name)
{
    this->points = points;
    this->name = name;
}

Polygon::Polygon(const Polygon &poly){
    this->points = poly.points;
    this->name = poly.name;
}

Polygon::~Polygon(void)
{
    
}

std::string Polygon::get_name() const
{
    return this->name;
}

std::vector<Point> Polygon::get_points() const
{
    return this->points;
}

void Polygon::set_name(std::string name)
{
    this->name = name;
}

void Polygon::set_points(std::vector<Point> points)
{
    this->points = points;
}

void Polygon::addPoint(Point point)
{
    this->points[points.size()] = point;
}

