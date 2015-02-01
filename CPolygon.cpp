//
//  Polygon.cpp
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//
#include "stdafx.h"
#include "CPolygon.h"

CPolygon::CPolygon()
{
    this->name = "Mon CPolygone";
}

CPolygon::CPolygon(const std::vector<Point> &points, std::string name)
{
    this->points = points;
    this->name = name;
}

CPolygon::CPolygon(const CPolygon &poly){
    this->points = poly.points;
    this->name = poly.name;
}

CPolygon::~CPolygon(void)
{
    
}

std::string CPolygon::get_name() const
{
    return this->name;
}

std::vector<Point> CPolygon::get_points() const
{
    return this->points;
}

void CPolygon::set_name(std::string name)
{
    this->name = name;
}

void CPolygon::set_points(std::vector<Point> points)
{
    this->points = points;
}

void CPolygon::addPoint(Point &point)
{
    this->points.push_back(point);
}

void CPolygon::clearPoints()
{
    this->points.clear();
}

std::ostream& operator<<(std::ostream& out, const CPolygon &p)
{
    for (std::size_t i = 0; i < p.get_points().size(); ++i) {
        out << p.get_points()[i] << " ";
    }
    
    return out;
}