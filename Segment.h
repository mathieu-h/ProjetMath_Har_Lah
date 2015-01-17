//
//  Segment.h
//  Projet Math
//
//  Created by Maxime Lahaye and Mathieu Harharidis.
//

#ifndef __Cours1__Segment__
#define __Cours1__Segment__

#include <stdio.h>
#include "Point.h"

class Segment
{
    
private:
    Point p1;
    Point p2;

public:
    /* Constructor and Destructor*/
    Segment();
    Segment(const Point &p1,const Point &p2);
    Segment(const Segment &s);
    ~Segment(void);
    
    /* Getter and Setter */
    Point get_p1() const;
    Point get_p2() const;
    void set_p1(const Point p1);
    void set_p2(const Point p2);
    
};

std::ostream& operator<<(std::ostream& out, const Segment &s);

#endif /* defined(__Cours1__Segment__) */
