//
//  Point.h
//  TP1
//
//  Created by Maxime Lahaye on 04/11/14.
//  Copyright (c) 2014 Maxime Lahaye. All rights reserved.
//

#ifndef __TP1__Point__
#define __TP1__Point__

#include <stdio.h>
#include <iostream>
#include <cmath>
class Point
{
private:
    float x_;
    float y_;
    
public:
    Point();
    Point(float x, float y);
    Point(const Point &p);
    ~Point(void);
    
    void add(const Point &p);
    void scale(float scale);
    float x_get() const;
    void x_set(float x);
    float y_get() const;
    void y_set(float y);
    
};

std::ostream& operator<<(std::ostream& out, const Point &p);


#endif /* defined(__TP1__Point__) */
