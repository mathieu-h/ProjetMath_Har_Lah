//
//  Segment.cpp
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#include "Segment.h"

Segment::Segment()
{
    
}


Segment::Segment(const Point &p1, const Point &p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

Segment::Segment(const Segment &s)
{
    this->p1 = s.p1;
    this->p2  = s.p2;
}

Segment::~Segment(void)
{
    delete &p1;
    delete &p2;
}

Point Segment::get_p1() const
{
    return this->p1;
}


Point Segment::get_p2() const
{
    return this->p2;
}

void Segment::set_p1(const Point p1)
{
    this->p1 = p1;
}

void Segment::set_p2(const Point p2)
{
    this->p2 = p2;
}

std::ostream& operator<<(std::ostream& out, const Segment &s)
{
    return out << "(" << s.get_p1() << "," << s.get_p2() << ")";
}