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

Polygon::Polygon(const std::list<Segment> &segments, std::string name)
{
    this->segments = segments;
    this->name = name;
}

Polygon::Polygon(const Polygon &poly){
    this->segments = poly.segments;
    this->name = poly.name;
}

Polygon::~Polygon(void)
{
    
}

std::string Polygon::get_name() const
{
    return this->name;
}

std::list<Segment> Polygon::get_segments() const
{
    return this->segments;
}

void Polygon::set_name(std::string name)
{
    this->name = name;
}

void Polygon::set_segments(std::list<Segment> segments)
{
    this->segments = segments;
}
