//
//  Segment.cpp
//  Cours1
//
//  Created by Maxime Lahaye on 17/01/15.
//  Copyright (c) 2015 Maxime Lahaye. All rights reserved.
//

#include "CVector.h"

CVector::CVector()
{
    
}


CVector::CVector(const Point &p1, const Point &p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

CVector::CVector(const CVector &c)
{
    this->p1 = c.p1;
    this->p2  = c.p2;
}

CVector::~CVector(void)
{
}

Point CVector::get_p1() const
{
    return this->p1;
}


Point CVector::get_p2() const
{
    return this->p2;
}

void CVector::set_p1(const Point p1)
{
    this->p1 = p1;
}

void CVector::set_p2(const Point p2)
{
    this->p2 = p2;
}

float CVector::diff_x() const
{
    return p2.x_get() - p1.x_get();
}

float CVector::diff_y() const
{
    return p2.y_get() - p1.y_get();
}

CVector CVector::normal() const
{
    Point p1(-diff_y(), diff_x());
    Point p2(diff_y(), -diff_x());
    return CVector(p1,p2);
}

std::ostream& operator<<(std::ostream& out, const CVector &c)
{
    return out << "(" << c.get_p1() << "," << c.get_p2() << ")";
}